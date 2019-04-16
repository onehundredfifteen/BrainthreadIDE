#pragma once

#include "ThreadState.h"
#include "../options/Language.h"

namespace BrainthreadIDE
{
    using namespace System;	
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;

	public ref class ThreadMonitor
	{
	public:	
		ThreadMonitor(const unsigned int& watch) 
			: current_thread_watch(watch) {
			this->threads = gcnew Dictionary<int, ThreadState ^>(); 
			this->thread_relations = gcnew Dictionary<int, int>();
		}

	public:
		property int CurrentThreadId {
			int get() { 
				return this->current_thread_watch; 
			}
		}

		property int Count {
			int get() { 
				return this->threads->Count; 
			}
		}

		property System::Collections::Generic::ICollection<ThreadState ^> ^ Items {
			System::Collections::Generic::ICollection<ThreadState ^> ^ get() { 
				return this->threads->Values; 
			}
		}

	public:
		bool IsWatched(int threadId) {
			return this->threads->ContainsKey(threadId);
		}

		bool IsEncountered(int threadId) {
			return this->thread_relations->ContainsKey(threadId);
		}

		ThreadState ^ CurrentThreadState() {	
			return this->GetThreadState(this->CurrentThreadId); 
		}

		ThreadState ^ GetThreadState(int threadId) {
			if(false == this->IsWatched(threadId)) {
					return nullptr;
			}
				
			return this->threads[threadId]; 
		}

		void Watch(ThreadState ^ watched_thread) {
			this->threads[ watched_thread->ThreadId ] = watched_thread;
		}

		void Encounter(int enc_thread_id);

		TreeNode ^ GetThreadInfoTreeNode(ThreadState ^ state, MemoryCellPrinter^ memPrinter, BrainthreadIDE::Language language);
		TreeNode ^ GetThreadChildrensTreeNode(ThreadState ^ state);
		TreeNode ^ GetSharedHeapTreeNode(MemoryCellPrinter^ memPrinter);

		String ^ GetThreadStatus(int threadId);
		String ^ GetThreadStatus(ThreadState ^ state);
		String ^ GetThreadName(int threadId);
		

	private:	
		Dictionary<int, ThreadState ^> ^ threads; //thread state by thread ID
		Dictionary<int, int> ^ thread_relations; //parent ID by thread ID

		const unsigned int& current_thread_watch;

	
	private:
		ThreadState ^ getThreadParent(int threadId);
		array<int> ^ getThreadChildrens(int threadId);

		int findOrphanKey();
		int findIconIndex(ThreadState ^ state);

		void fillChildrenTreeNode(TreeNode ^ mainNode, int parentThreadId);
		TreeNode ^ createTreeNodeFromArray(array<String^>^ data_array);
		
	private: 
		static const int cNoParentThread = -1;
		static const int cOrphanedThread = 0;

	};
}