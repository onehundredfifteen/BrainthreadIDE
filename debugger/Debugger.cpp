#include "stdafx.h"
#include <vcclr.h>

#include "Debugger.h"
#include "DebugTreeViewExpander.h"
#include "../options/GlobalOptions.h"

namespace BrainthreadIDE
{

	Debugger::Debugger(String ^ exe_path, String ^ args, BrainthreadIDE::Language lang)
	{
		args = " " + args; //! trailing space before args

		//String ^ to wchar_t*
		pin_ptr<const wchar_t> pexe_path = PtrToStringChars(exe_path); 
		pin_ptr<const wchar_t> parg = PtrToStringChars(args);

		this->debuggerLoop = nullptr;
		this->memoryForImage = 120;
		this->memoryTableCols = 5;
		ProcessProperties.code_pointer_pos = 0;

		this->language = lang;
		this->memPrinter = gcnew MemoryCellPrinter;
		this->threadMonitor = gcnew ThreadMonitor(BrainthreadIDE::ProcessProperties.thread_id);
		
		try
		{
			this->debuggerLoop = new DebuggerLoop(const_cast<wchar_t*>(pexe_path), const_cast<wchar_t*>(parg));	
		}
		catch(DebugTimeException &de) {
			throw gcnew Exception(gcnew String(de.what())); 
		}
		catch(...) {
			throw gcnew Exception("Unknown exception was raised while starting debugger"); 
		}
	}

	Debugger::!Debugger()
	{
		if(this->debuggerLoop)
			delete this->debuggerLoop;
	}
	Debugger::~Debugger()
	{
		this->!Debugger();
	}

	void Debugger::RunStep()
	{	
		try
		{
			debuggerLoop->Step(0);	
			debuggerLoop->FillProcessProperties(ProcessProperties);	

			//saving data
		    UpdateDataFromMemory();
		}
		catch(LanguageException &le) {
			MessageBox::Show(gcnew String(le.what()), "problem with code", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			//throw gcnew Exception(gcnew String(le.what())); 
		}
		catch(DebugTimeException &de) {
			throw gcnew Exception(gcnew String(de.what())); 
		}
		catch(...){
			throw gcnew Exception("Unknown exception was raised while 'run-one-step' command"); 
		}
	}

	void Debugger::RunToInstruction(int instruction_pos)
	{
		try
		{
			debuggerLoop->Step(instruction_pos);	
		    debuggerLoop->FillProcessProperties(ProcessProperties);	

			//saving data
		    UpdateDataFromMemory();
		}
		catch(LanguageException &le) {
			MessageBox::Show(gcnew String(le.what()), "problem with code", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			//throw gcnew Exception(gcnew String(le.what())); 
		}
		catch(DebugTimeException &de) {
			throw gcnew Exception(gcnew String(de.what())); 
		}
		catch(...) {
			throw gcnew Exception("Unknown exception was raised while 'run-to-instruction' command"); 
		}
	}

	void Debugger::RunToMemoryTrap(int memory_index, int value, CompareType compare)
	{ //mem index zero-based
		try
		{
			bool trapped = false;
			array<int> ^ memory;

			do
			{
				debuggerLoop->Step(0);	
				debuggerLoop->FillProcessProperties(ProcessProperties);	

				if(memory_index >= 0) //single value
				{
					memory = this->getMemoryImage(memory_index + 1);
					trapped = compareValues(memory[memory_index], value, compare);
				}
				else //all cells
				{
					int lnz_cell = ProcessProperties.meminfo.last_nonzero_cell;
					memory = this->getMemoryImage(lnz_cell + 1);

					for(int i = 0; i <= lnz_cell; ++i)
					{
						trapped = compareValues(memory[i], value, compare);

						if(trapped)
							break;
					}
				}
			}
			while(false == trapped && debuggerLoop->IsDebugeeRunning());

			//saving data
			UpdateDataFromMemory();

			if(false == trapped)
			{
				MessageBox::Show("Program ended before falling into a memory trap", "Conditions not met", MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
		}
		catch(LanguageException &le) {
			MessageBox::Show(gcnew String(le.what()), "problem with code", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			//throw gcnew Exception(gcnew String(le.what())); 
		}
		catch(DebugTimeException &de) {
			throw gcnew Exception(gcnew String(de.what())); 
		}
		catch(...) {
			throw gcnew Exception("Unknown exception was raised while 'run-to-trap' command"); 
		}
	}

	void Debugger::Finish()
	{
		this->debuggerLoop->Terminate();	
		//debuggerLoop->FillProcessProperties(ProcessProperties);
	}

	void Debugger::DetachDebugee(bool kill_debugee)
	{
		if(true == kill_debugee) {
			this->Finish();		
		}
		else
			this->debuggerLoop->Detach();	
		//debuggerLoop->FillProcessProperties(ProcessProperties);
	}

	void Debugger::UpdateDataFromMemory()
	{
		//saving data by thread
		ThreadState ^ current_thread;
		bool just_spawned = false == this->threadMonitor->IsWatched(ProcessProperties.thread_id);

		if(just_spawned) {

			this->threadMonitor->Encounter(ProcessProperties.thread_id);
			current_thread = gcnew ThreadState(&ProcessProperties, this->getMemoryImage(this->memoryForImage));
			
			//handle 'trace new thrad' option -> not working for main thread 
			if(GlobalOptions::Instance->TraceNewThread == false && this->threadMonitor->Count > 1)
			{
				current_thread->Trace = false;
				this->debuggerLoop->TraceThread(current_thread->ThreadId, false);
			}
		}
		else {
			current_thread = gcnew ThreadState(this->threadMonitor->CurrentThreadState(), &ProcessProperties, this->getMemoryImage(this->memoryForImage)); 
		}

		//keep data saved
		this->threadMonitor->Watch(current_thread);

		//encounter children threads
		for(int i = 0; i < ProcessProperties.threadinfo.array_size; ++i) {
			this->threadMonitor->Encounter(ProcessProperties.threadinfo.array_memory[i]);
		}
		//update other threads
		this->updateIdleThreadsProgress();
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

			tab_cols = ((dgvSize->Width - first_row_width ) / 30);
		}
		
		this->memoryTableCols = tab_cols;

		if(CurrentThreadId)
			LoadMemoryImageToView(dataGridView, CurrentThreadId);
	}

	void Debugger::LoadMemoryImageToView(DataGridView ^ dataGridView, int threadId)
	{
		//creating table
		DataTable ^ dataSource = gcnew DataTable;

		if(false == this->threadMonitor->IsWatched(threadId))
			throw gcnew ArgumentException(String::Format("Thread {0} is not accesible now", threadId));

		array<int> ^ memImage = this->threadMonitor->GetThreadState(threadId)->MemoryImage;

		int col_cnt = this->memoryTableCols <= 0 ? 5 : this->memoryTableCols;
		int row_cnt = memImage->Length / this->memoryTableCols + ((memImage->Length % this->memoryTableCols)? 1 : 0);

		//converting
		for(int i = 0; i < col_cnt; ++i)
			dataSource->Columns->Add( gcnew DataColumn( String::Format("+{0}", i+1)));

		for(int j = 0; j < row_cnt; ++j)
		{
			DataRow ^ newRow = dataSource->NewRow();

			// Set values in the columns:
			for(int i = 0; i < col_cnt && (j * col_cnt + i) < memImage->Length; ++i) {
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
		TreeViewExpander ^ treeViewExpander = gcnew TreeViewExpander(threadTreeView);
		
		threadTreeView->BeginUpdate();
		threadTreeView->Nodes->Clear();
		
		if(GlobalOptions::Instance->ThreadTreeIcons != 0)
			threadTreeView->ImageList = nullptr;

		//thread tree
		for each(ThreadState ^ state in this->threadMonitor->Items) {
			threadTreeView->Nodes->Add(this->threadMonitor->GetThreadInfoTreeNode(state, this->memPrinter, this->language));
		}

		if(this->language == Language::lBrainthread || this->language == Language::lPBrain) {
			//functions TODO
			threadTreeView->Nodes->Add(String::Format("Functions in the scope {0}", ProcessProperties.funinfo.loaded_functions));
		}

		//shared stack
		if(this->language == Language::lBrainthread) {
			threadTreeView->Nodes->Add( this->threadMonitor->GetSharedHeapTreeNode(this->memPrinter)); 
		}

		//expanding nodes by language
		if(this->language == Language::lBrainthread || this->language == Language::lBrainfork)
			treeViewExpander->Expand();
		else
			treeViewExpander->ExpandAll();

		threadTreeView->EndUpdate();
	}

	void Debugger::LoadThreadList(ComboBox ^ threadListComboBox)
	{
		DataTable ^ dtThreads = gcnew DataTable("threads");
		dtThreads->Columns->Add( gcnew DataColumn("text") );
		dtThreads->Columns->Add( gcnew DataColumn("sys_id") );

		for each(ThreadState ^ state in this->threadMonitor->Items) {
			dtThreads->Rows->Add(this->threadMonitor->GetThreadStatus(state), state->ThreadId);
		}

		threadListComboBox->DataSource = dtThreads;
		threadListComboBox->DisplayMember = "text";
		threadListComboBox->ValueMember = "sys_id";

		threadListComboBox->SelectedValue = this->threadMonitor->CurrentThreadId;//comboBoxThreadMemSelect->SelectedValuecurrent_thread_index;	///tu ominac warunek by index TODO
	}

	void Debugger::LoadStatus(Label ^ label)
	{
		ThreadState ^ cur_state = this->threadMonitor->CurrentThreadState();
		if(false == debuggerLoop->IsDebugeeRunning() || nullptr == cur_state)
			label->Text = "Debugger is not running";
		else
			label->Text =  ( this->threadMonitor->Count == 1 ? "Main thread " : "Thread ") + this->threadMonitor->GetThreadName(cur_state->ThreadId) + " at " + cur_state->Synthesis();

		label->Tag = label->Text; //handy copy for visual ease
	}

	void Debugger::WriteMemoryToThread(String ^ value, int index, int threadId)
	{
		IntegerDigest ^ printer = gcnew IntegerDigest(gcnew MemoryCellPrinter());

		if(false == this->threadMonitor->IsWatched(threadId)) {
			MessageBox::Show(String::Format("Cannot write to thread {0}.\nThis thread isn't initialized yet.", this->threadMonitor->GetThreadName(threadId)), "Writing to memory",
												MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		}
		else if(false == this->ThreadTracing[threadId]) {
			MessageBox::Show(String::Format("Cannot write to thread {0}.\nThis thread isn't traced or has been exited.", this->threadMonitor->GetThreadName(threadId)), "Writing to memory",
												MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		}
		else if(index < 0 || index >= this->getMemoryLen()) {
			MessageBox::Show(String::Format("Cannot write '{0}' as '{1}' to memory at position {2}.\nThe position is out of bounds.", value, printer->Digest(value), index), "Writing to memory",
												MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		}
		else if(false == this->debuggerLoop->WriteProcessMemory(printer->Digest(value), index, threadId)) { 
			MessageBox::Show(String::Format("Cannot write '{0}' as '{1}' to memory at position {2}.\nYour value isn't stored.", value, printer->Digest(value), index), "Writing to memory",
												MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		}
	}

	String ^ Debugger::GetDebugStats()
	{
		/*int total_executed_cnt = 0;

		StringBuilder ^ sb = gcnew StringBuilder();
		for each(ThreadState ^ state in this->threadMonitor->Items)
		{
			int steps_executed = this->getThreadSteps(state->ThreadId);
			sb->Append(String::Format("\tThread {0}: {1} steps were executed.\r\n", this->getThreadName(state->ThreadId), steps_executed));
			
			total_executed_cnt += steps_executed;
		}

		sb->Insert(0, String::Format("Total {0} instructions executed in {1} thread(s):\r\n", total_executed_cnt, this->threadMonitor->Count));
		*/
		return "" ;// sb->ToString();*/
	}

	void Debugger::ExceptionDumpToThreadTree(TreeView ^ threadTreeView)
	{ //todo: return array of string to put in other controls
		threadTreeView->BeginUpdate();
		threadTreeView->Nodes->Clear();

		//DebugeeExitCode 
		threadTreeView->Nodes->Add(String::Format("Debugee exit code: {0}", this->DebugeeExitCode));
		threadTreeView->Nodes->Add(String::Format("Invoked threads: {0}", this->threadMonitor->Count)); 
		threadTreeView->Nodes->Add(String::Format("Current thread ID: {0}", this->threadMonitor->CurrentThreadId));
		//threadTreeView->Nodes->Add("Current thread ID: " + this->threadMonitor->CurrentThreadId.ToString());
		//threadTreeView->Nodes->Add("Current thread ID: " + this->threadMonitor->CurrentThreadId.ToString());
		threadTreeView->Nodes->Add(String::Format("dp_id={0}", this->DebugeeProcessId));
		//threadTreeView->Nodes->Add(String::Format("dl_addr={0}", ((int)this->debuggerLoop));

		//threadTreeView->Nodes->Add(String::Format("tti={0}", (ProcessProperties.threadinfo));
		//threadTreeView->Nodes->Add(String::Format("Invoked threads: {0}", this->threadMonitor->Count));
		
		threadTreeView->EndUpdate();
	}

	

	//debugger internal methods

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
				debuggerLoop->FillProcessProperties(ProcessProperties, n);
				n = 0;
				++chunks;
			}

			dataImage[i] = ProcessProperties.meminfo.memory_chunk[n];
			++n;
		}

		return dataImage;
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
			//col->HeaderCell->Style->Font = (gcnew System::Drawing::Font(L"Lucida Sans Unicode", 10.0F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				//static_cast<System::Byte>(0))); //238
	    }
		
		//resize columns
        dataGridView->AutoResizeRowHeadersWidth( DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders );
		dataGridView->AutoResizeColumns( DataGridViewAutoSizeColumnsMode::AllCells );
	}

	void Debugger::updateThreadTracing()
	{
		for each(ThreadState ^ state in this->threadMonitor->Items) {
			debuggerLoop->TraceThread(state->ThreadId, state->Trace);
		}						
	}

	void Debugger::updateIdleThreadsProgress()
	{
		for each(ThreadState ^ state in this->threadMonitor->Items) {
			if(state != this->threadMonitor->CurrentThreadState()) {
				state->StepsExecuted = this->debuggerLoop->GetThreadSteps(state->ThreadId);
			}	
		}					
	}

	bool Debugger::compareValues(int a, int b, Debugger::CompareType compare)
	{
		switch(compare)
		{
			case CompareType::NotEqual:
					return a != b;
			case CompareType::Smaller:
					return a < b;
			case CompareType::Bigger:
					return a > b;
			case CompareType::Equal:
			default:
				return a == b;
		}
	}
	
	int Debugger::getMemoryLen()
	{
		return BrainthreadIDE::ProcessProperties.meminfo.memory_len;
	}

}