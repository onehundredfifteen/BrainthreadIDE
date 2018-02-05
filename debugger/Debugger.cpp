#include "stdafx.h"
#include <vcclr.h>

#include "Debugger.h"
#include "DebugTreeViewExpander.h"
#include "../options/GlobalOptions.h"

namespace BrainthreadIDE
{

	Debugger::Debugger(String ^ exe_path, String ^ args)
	{
		args = " " + args; //! trailing space before args

		//String ^ to wchar_t*
		pin_ptr<const wchar_t> pexe_path = PtrToStringChars(exe_path); 
		pin_ptr<const wchar_t> parg = PtrToStringChars(args);

		this->debuggerLoop = nullptr;
		this->memoryForImage = 120;
		this->memoryTableCols = 5;
		ProcessProperties.code_pointer_pos = 0;

		this->memPrinter = gcnew MemoryCellPrinter;
		this->threadResource = gcnew Dictionary<int, ThreadResourceState ^>();
		
		try
		{
			this->debuggerLoop = new DebuggerLoop(const_cast<wchar_t*>(pexe_path), const_cast<wchar_t*>(parg));	
		}
		catch(DebugTimeException &de)
		{
			throw gcnew Exception(gcnew String(de.what())); 
		}
		catch(...)
		{
			throw gcnew Exception("Unknown exception was raised while starting debugger"); 
		}
	}

	Debugger::~Debugger()
	{
		//if(this->debuggerLoop)
			//delete this->debuggerLoop;
	}

	void Debugger::RunStep()
	{	
		try
		{
			debuggerLoop->Step(0);	
			debuggerLoop->GetProcessProperties(ProcessProperties);	

			//saving data
		    LoadMemoryByThread();
		}
		catch(DebugTimeException &de)
		{
			throw gcnew Exception(gcnew String(de.what())); 
		}
		catch(...)
		{
			throw gcnew Exception("Unknown exception was raised while 'run-one-step' command"); 
		}
	}

	void Debugger::RunToInstruction(int instruction_pos)
	{
		try
		{
			debuggerLoop->Step(instruction_pos);	
		    debuggerLoop->GetProcessProperties(ProcessProperties);	

			//saving data
		    LoadMemoryByThread();
		}
		catch(DebugTimeException &de)
		{
			throw gcnew Exception(gcnew String(de.what())); 
		}
		catch(...)
		{
			throw gcnew Exception("Unknown exception was raised while 'run-to-instruction' command"); 
		}
	}

	void Debugger::Finish()
	{
		debuggerLoop->Terminate();	
		//debuggerLoop->GetProcessProperties(ProcessProperties);
	}

	void Debugger::DetachDebugee()
	{
		debuggerLoop->Detach();	
		//debuggerLoop->GetProcessProperties(ProcessProperties);
	}

	void Debugger::LoadMemoryByThread()
	{
		//saving data by thread
		bool just_spawned = false == this->threadResource->ContainsKey(ProcessProperties.thread_id);

		this->threadResource[ ProcessProperties.thread_id ] = gcnew ThreadResourceState(this->getThreadInfoTreeNode(), 
																						this->getMemoryImage(this->memoryForImage),
																						ProcessProperties.code_pointer_pos,
																						ProcessProperties.meminfo.pointer_pos,
																						ProcessProperties.meminfo.last_nonzero_cell
																						);
		//handle 'trace new thrad' option -> not working for main thread 
		if(just_spawned == true && GlobalOptions::Instance->TraceNewThread == false && this->threadResource->Count > 1)
		{
			this->threadResource[ ProcessProperties.thread_id ]->Trace = false;
			debuggerLoop->TraceThread(ProcessProperties.thread_id, false);
		}
			
	}

	void Debugger::LoadMemoryImageToView(DataGridView ^ dataGridView)
	{
		LoadMemoryImageToView(dataGridView, CurrentThreadId);
	}

	void Debugger::LoadMemoryImageToView(DataGridView ^ dataGridView, MemoryCellPrinter ^ memPrinter)
	{
		this->memPrinter = memPrinter;

		if(CurrentThreadId)
			LoadMemoryImageToView(dataGridView, CurrentThreadId);
	}

	void Debugger::LoadMemoryImageToView(DataGridView ^ dataGridView, Int16 tab_cols)
	{
 		if(tab_cols == 0) //fit columns to width
		{
			int first_row_width;
			System::Drawing::Size ^ dgvSize = dataGridView->ClientSize;

			first_row_width = (dataGridView->Columns->Count > 0)? dataGridView->RowHeadersWidth : 80;

			tab_cols = ((dgvSize->Width - first_row_width ) / 30) ;
		}
		
		this->memoryTableCols = tab_cols;

		if(CurrentThreadId)
			LoadMemoryImageToView(dataGridView, CurrentThreadId);
	}

	void Debugger::LoadMemoryImageToView(DataGridView ^ dataGridView, int threadId)
	{
		//creating table
		DataTable ^ dataSource = gcnew DataTable;
		array<int> ^ memImage = this->threadResource[ threadId ]->MemoryImage;

		int col_cnt = this->memoryTableCols <= 0 ? 5 : this->memoryTableCols;
		int row_cnt = memImage->Length / this->memoryTableCols + ((memImage->Length % this->memoryTableCols)? 1 : 0);

		//converting
		for(int i = 0; i < col_cnt; ++i)
			dataSource->Columns->Add( gcnew DataColumn( String::Format("+{0}", i+1)));

		for(int j = 0; j < row_cnt; ++j)
		{
			DataRow ^ newRow = dataSource->NewRow();

			// Set values in the columns:
			for(int i = 0; i < col_cnt && (j * col_cnt + i) < memImage->Length; ++i)
			{
				newRow[i] = this->memPrinter->Print( memImage[j * col_cnt + i] );
			}
			
			// Add the row to the rows collection.
			dataSource->Rows->Add(newRow);
		}

		//setting data
		dataSource->Locale = System::Globalization::CultureInfo::InvariantCulture;
		dataGridView->DataSource = dataSource;
		
		//setting row & col headers
	    this->customizeView(dataGridView);
	}

	

	void Debugger::LoadThreadTree(TreeView ^ threadTreeView)
	{
		DebugTreeViewExpander ^ treeViewExpander = gcnew DebugTreeViewExpander(threadTreeView);
		Dictionary<int, ThreadResourceState ^>::Enumerator etor = threadResource->GetEnumerator();
		//TreeNode ^ old_selected_node = threadTreeView->SelectedNode; !!!!!!!!!
		
		threadTreeView->BeginUpdate();
		threadTreeView->Nodes->Clear();

		while(etor.MoveNext())
		{
			etor.Current.Value->MemoryInfoNode->BackColor = System::Drawing::SystemColors::Window;

			if(etor.Current.Value->Trace == false)
				threadTreeView->Nodes->Add(this->getDetachedThreadInfoTreeNode(etor.Current.Key));
			else if(debuggerLoop->IsThreadRunning(etor.Current.Key) == false)
				threadTreeView->Nodes->Add(this->getExitedThreadInfoTreeNode(etor.Current.Key));
			else
				threadTreeView->Nodes->Add(etor.Current.Value->MemoryInfoNode); 

			if(etor.Current.Key == getCurrentThreadId())
				etor.Current.Value->MemoryInfoNode->BackColor = System::Drawing::Color::Yellow;
		}

		threadTreeView->Nodes->Add(String::Format("Functions call stack {0}", 0)); 
		threadTreeView->Nodes->Add(String::Format("Shared heap size: {0}", ProcessProperties.shstackinfo.list_size)); 
		
		treeViewExpander->Expand();
		threadTreeView->EndUpdate();
	}

	void Debugger::LoadThreadList(ComboBox ^ threadListComboBox)
	{
		int i = 0, current_thread_index = 0;
		DataTable ^ dtThreads = gcnew DataTable("threads");
		Dictionary<int, ThreadResourceState ^>::Enumerator etor = threadResource->GetEnumerator();

		dtThreads->Columns->Add( gcnew DataColumn( "text" ) );
		dtThreads->Columns->Add( gcnew DataColumn( "value" ) );

		while(etor.MoveNext())
		{
			if(etor.Current.Key == getCurrentThreadId())
			{
				current_thread_index = i;
				dtThreads->Rows->Add(String::Format("Thread {0} (Current)", etor.Current.Key), etor.Current.Key);
			}
			else 
				dtThreads->Rows->Add(String::Format("Thread {0}", etor.Current.Key), etor.Current.Key);
			++i;
		}

		threadListComboBox->DataSource = dtThreads;
		threadListComboBox->DisplayMember = "text";
		threadListComboBox->ValueMember = "value";

		threadListComboBox->SelectedIndex = current_thread_index;	
	}

	void Debugger::LoadStatus(Label ^ label)
	{
		if(threadResource->Count == 1)
			label->Text = String::Format("Main thread on instruction {0}, executed: {1}", ProcessProperties.code_pointer_pos,  ProcessProperties.steps_executed);
		else
			label->Text = String::Format("Thread {0} on instruction {1}, executed: {2}", 
										getCurrentThreadId(), 
										ProcessProperties.code_pointer_pos, 
										ProcessProperties.steps_executed);

		label->Tag = label->Text; //copy
	}

	array<int> ^ Debugger::getMemoryImage(int last_cell_to_load)
	{
		int n = 0, chunks = 1;
		int loaded_memory_len = Math::Min(last_cell_to_load, (int)ProcessProperties.meminfo.memory_len); 

		array<int> ^ dataImage = gcnew array<int>(loaded_memory_len);

		for(int i = 0; i < loaded_memory_len; ++i)
		{
			if(n >= MEM_CHUNK_SIZE && chunks * n < loaded_memory_len) 
			{
				//load new chunk
				debuggerLoop->GetProcessProperties(ProcessProperties, n);
				n = 0;
				++chunks;
			}

			dataImage[i] = ProcessProperties.meminfo.memory_chunk[n];
			++n;
		}

		return dataImage;
	}
	
	TreeNode ^ Debugger::getThreadInfoTreeNode()
	{
		TreeNode ^mainNode, ^memoryNode, ^stackNode, ^threadsNode, ^childNode;

		array<MemoryCellPrinter^> ^ memPrinters = gcnew array<MemoryCellPrinter^>{gcnew MemoryCellPrinter(), gcnew CharacterCellPrinter(), gcnew HexCellPrinter()};
		
		mainNode = gcnew TreeNode( String::Format("Thread {0}: instruction: {1} steps: {2}", ProcessProperties.thread_id , 
																							 ProcessProperties.code_pointer_pos, 
																							 ProcessProperties.steps_executed));
		//memory
		memoryNode = gcnew TreeNode( String::Format("Memory position {0}", ProcessProperties.meminfo.pointer_pos+1 ));

		
		memoryNode->Nodes->Add( String::Format("Current cell value: {0} '{1}' 0x{2}", memPrinters[0]->Print( ProcessProperties.meminfo.current_cell_value ),
																		 memPrinters[1]->Print( ProcessProperties.meminfo.current_cell_value ),
																		 memPrinters[2]->Print( ProcessProperties.meminfo.current_cell_value )));

		memoryNode->Nodes->Add( String::Format("Last non-zero cell position: {0}", ProcessProperties.meminfo.last_nonzero_cell+1));
		memoryNode->Nodes->Add( String::Format("Memory size: {0}", ProcessProperties.meminfo.memory_len));
		
		//stack
		stackNode = gcnew TreeNode( String::Format("Heap size: {0}", ProcessProperties.stackinfo.list_size ));

		//child threads
		threadsNode = gcnew TreeNode( String::Format("Child threads: {0}", ProcessProperties.threadinfo.list_size ));
		/*
		for(int i = 0; i < ProcessProperties.threadinfo.list_size; ++i)
		{
			childNode = gcnew TreeNode( String::Format("Child thread {0}", ProcessProperties.threadinfo.list_memory[i]) );
			childNode->Tag = ProcessProperties.threadinfo.list_memory[i];
			threadsNode->Nodes->Add(childNode);
		}
		*/


		mainNode->Nodes->Add( memoryNode);
		mainNode->Nodes->Add( stackNode);
		mainNode->Nodes->Add( threadsNode);

		mainNode->Tag = static_cast<int>(ProcessProperties.thread_id);

		return mainNode;
	}

	TreeNode ^ Debugger::getDetachedThreadInfoTreeNode(int threadId)
	{
		TreeNode ^ mainNode = gcnew TreeNode( String::Format("Thread {0}: steps: {1} DETACHED", 
															 threadId, 
															 debuggerLoop->GetThreadSteps(threadId)));
		
		mainNode->Tag = static_cast<int>(threadId);
		return mainNode;
	}

	TreeNode ^ Debugger::getExitedThreadInfoTreeNode(int threadId)
	{
		TreeNode ^ mainNode = gcnew TreeNode( String::Format("Thread {0} EXITED", threadId) );
		
		mainNode->Tag = static_cast<int>(threadId);
		return mainNode;
	}

	void Debugger::customizeView(DataGridView ^ dataGridView)
	{
		int i = 0, rowNumber = 0;

		//setting row & col headers
	    for each(DataGridViewRow ^ row in dataGridView->Rows) 
	    {
			if ( row->IsNewRow )
				   continue;

			row->HeaderCell->Value = (rowNumber * this->memoryTableCols).ToString();
			++rowNumber;
	    }

	    for each(DataGridViewColumn ^ col in dataGridView->Columns) 
	    {
			col->SortMode = DataGridViewColumnSortMode::NotSortable;
	    }
		
		//resize columns
        dataGridView->AutoResizeRowHeadersWidth( DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders );
		dataGridView->AutoResizeColumns( DataGridViewAutoSizeColumnsMode::AllCells );
	}

	void Debugger::updateTracing()
	{
		Dictionary<int, ThreadResourceState ^>::Enumerator etor = threadResource->GetEnumerator();
		
		while(etor.MoveNext())
		{
			debuggerLoop->TraceThread(etor.Current.Key, etor.Current.Value->Trace);
		}								
	}
	/*
	int Debugger::getCodePos()
	{
		return BrainthreadIDE::ProcessProperties.code_pointer_pos;
	}
	int Debugger::getMemoryPos()
	{
		return BrainthreadIDE::ProcessProperties.meminfo.pointer_pos;
	}
	int Debugger::getMemoryLNZPos()
	{
		return BrainthreadIDE::ProcessProperties.meminfo.last_nonzero_cell;
	}*/
	int Debugger::getMemoryLen()
	{
		return BrainthreadIDE::ProcessProperties.meminfo.memory_len;
	}
	
	int Debugger::getCurrentThreadId()
	{
		return ProcessProperties.thread_id;
	}

}