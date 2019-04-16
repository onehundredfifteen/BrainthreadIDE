#include "stdafx.h"
#include <vcclr.h>

#include "ThreadMonitor.h"
#include "../options/GlobalOptions.h"

namespace BrainthreadIDE
{
	void ThreadMonitor::Encounter(int enc_thread_id)
	{
		//Holds relation [child] => parent as many childs can have only one parent
			
		//if there is the root 
		if(this->thread_relations->Count == 0)
			this->thread_relations[ enc_thread_id ] = cNoParentThread;
		else if(this->IsEncountered(enc_thread_id)) {
			//ThreadState ^ parent = this->getThreadParent(enc_thread_id);
			//this->thread_relations[ enc_thread_id ] = parent ? parent->ThreadId : this->CurrentThreadId;
			return;
		}
		else if(enc_thread_id == this->CurrentThreadId) {
			if(this->IsEncountered(cOrphanedThread)) {
				this->thread_relations[ enc_thread_id ] = this->thread_relations[ cOrphanedThread ];
				this->thread_relations->Remove(cOrphanedThread);
			}
			else
				throw gcnew Exception("Information about the hierarchy of the thread " + this->GetThreadName(enc_thread_id) + " may be corrupted");
		}
		else {//if(enc_thread_id == this->CurrentThreadId)
			this->thread_relations[ enc_thread_id ] = this->CurrentThreadId;
		}
	}
	
	TreeNode ^ ThreadMonitor::GetThreadInfoTreeNode(ThreadState ^ state, MemoryCellPrinter^ memPrinter, BrainthreadIDE::Language language)
	{
		int ic_idx = this->findIconIndex(state);
		TreeNode ^ mainNode = gcnew TreeNode( this->GetThreadStatus(state) + " " + state->Synthesis(), ic_idx, ic_idx );
		
		//memory nodes
		mainNode->Nodes->Add( this->createTreeNodeFromArray( state->MemorySynthesis() ));

		//heap node
		if(language == Language::lBrainthread)
			mainNode->Nodes->Add( this->createTreeNodeFromArray( state->HeapSynthesis(memPrinter) ));

		//callstack
		if(language == Language::lBrainthread || language == Language::lPBrain)
			mainNode->Nodes->Add( this->createTreeNodeFromArray( state->FunctionSynthesis(memPrinter) ));

		//child threads
		if(language == Language::lBrainthread || language == Language::lBrainfork) 
			mainNode->Nodes->Add( this->GetThreadChildrensTreeNode(state));
		
		//styling
		if(GlobalOptions::Instance->ThreadTreeIcons == 1 && state == this->CurrentThreadState())
			mainNode->BackColor = System::Drawing::Color::LightBlue;

		mainNode->Tag = static_cast<int>(state->ThreadId);

		return mainNode;
	}

	TreeNode ^ ThreadMonitor::GetThreadChildrensTreeNode(ThreadState ^ state)
	{
		TreeNode ^mainNode;
		array<int> ^ childs = this->getThreadChildrens(state->ThreadId);

		ThreadState ^ parent = this->getThreadParent(state->ThreadId);
		mainNode = gcnew TreeNode( String::Format("Child threads: {0} {1}", childs->Length, parent ? "Parent: "+ this->GetThreadName(parent->ThreadId) : "" ));

		this->fillChildrenTreeNode(mainNode, state->ThreadId);

		return mainNode;
	}

	TreeNode ^ ThreadMonitor::GetSharedHeapTreeNode(MemoryCellPrinter^ memPrinter)
	{
		array<String^> ^ data = this->CurrentThreadState()->SharedHeapSynthesis(memPrinter);
		
		if(data->Length > 0)
				data[0] = data[0]->Replace("Heap","Shared heap");

		return this->createTreeNodeFromArray(data);
	}

	String ^ ThreadMonitor::GetThreadStatus(int threadId)
	{
		String ^ result;
		ThreadState ^ state = this->GetThreadState(threadId);
		
		if(state == nullptr) {
			if(this->IsEncountered(threadId))
				result = "Spawning";
			else
				result = "Rogue";
		}
		else 
			return this->GetThreadStatus(state);

		return String::Format("Thread {0} ({1})", this->GetThreadName(threadId), result);
	}

	String ^ ThreadMonitor::GetThreadStatus(ThreadState ^ state)
	{
		String ^ result;
		array<Char>^ charsToTrim = { L',', L' '};
		System::Collections::IEnumerator ^ etor = this->Items->GetEnumerator();
		
		if(etor->MoveNext() && state->ThreadId == cli::safe_cast<ThreadState ^>(etor->Current)->ThreadId)
				result += " Root,";

		if(state == this->CurrentThreadState())
				result +=" Current,";

		if(false == state->Trace)
				result += " Detached,";

		if(false == state->Alive)
				result += " Exited,";

		if(String::IsNullOrEmpty(result))
			return "Thread " + this->GetThreadName(state->ThreadId);

		return String::Format("Thread {0} ({1})", this->GetThreadName(state->ThreadId) , result->Trim(charsToTrim));
	}
	
	String ^ ThreadMonitor::GetThreadName(int threadId)
	{
		if(false == this->IsEncountered(threadId)) //raw sys thread id
			return "?" + threadId.ToString() + "?";
		else if(cOrphanedThread == threadId)
			return " is spawning...";
		else if(false == this->IsWatched(threadId))
			return "[" + threadId.ToString() + "]";	

		if(GlobalOptions::Instance->ThreadNaming == 0) //sys thread id
			return threadId.ToString();
		else //by order 
		{
			int t = 0; //order cnt
			for each(ThreadState ^ state in this->Items) {
				if(state->ThreadId == threadId)
					break;
				++t;
			}

			if(GlobalOptions::Instance->ThreadNaming == 2)
				t += 1;
			
			return t.ToString();
		}
	}

	//private methods

	ThreadState ^ ThreadMonitor::getThreadParent(int threadId)
	{
		int parent_thread_id = this->thread_relations[ threadId ];

		if(parent_thread_id != cNoParentThread)
			return this->GetThreadState(parent_thread_id);
		else
			return nullptr;
	}

	array<int> ^ ThreadMonitor::getThreadChildrens(int threadId) 
	{
		Dictionary<int, int>::Enumerator etor = this->thread_relations->GetEnumerator();
		System::Collections::Generic::List<int> ^ childrens = gcnew System::Collections::Generic::List<int>();

		while(etor.MoveNext()) {
			if(etor.Current.Value == threadId) {
				childrens->Add(etor.Current.Key); //collect childs from parent id
			}
		}

		return childrens->ToArray();
	}

	//some early spawned threads may have id = 0, get that thread
	int ThreadMonitor::findOrphanKey() 
	{
		return this->IsEncountered(cOrphanedThread);
			/*return this->thread_relations
		else
			return cNoParentThread;*/
	}

	void ThreadMonitor::fillChildrenTreeNode(TreeNode ^ mainNode, int parentThreadId)
	{
		array<int> ^ childs = this->getThreadChildrens(parentThreadId);

		for each(int ch_tid in childs) {
			TreeNode ^ childNode = gcnew TreeNode( "Thread " + this->GetThreadName(ch_tid) );
			
			if(this->IsWatched(ch_tid)) { 

				this->fillChildrenTreeNode(childNode, ch_tid);
				childNode->Text += (" Entrypoint: " + this->GetThreadState(ch_tid)->Entrypoint);

				int ic_idx = this->findIconIndex(this->GetThreadState(ch_tid));
				childNode->ImageIndex = ic_idx;
				childNode->SelectedImageIndex = ic_idx;
			}

			childNode->Tag = static_cast<int>(ch_tid);
			mainNode->Nodes->Add(childNode);
		}
	}

	TreeNode ^ ThreadMonitor::createTreeNodeFromArray(array<String^>^ data_array)
	{
		TreeNode ^ node = gcnew TreeNode( data_array[0] );

		for (int i = 1; i < data_array->Length; i++)
			node->Nodes->Add( data_array[i] );

		return node;
	}

	int ThreadMonitor::findIconIndex(ThreadState ^ state)
	{
		if(state->ThreadId == this->CurrentThreadId)
			return 1;
		else if(false == state->Trace)
			return 3;
		else if(false == state->Alive)
			return 4;

		return 2;
	}

	
}