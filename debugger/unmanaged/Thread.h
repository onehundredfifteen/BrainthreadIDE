#pragma once

#include <vector>
#include <windows.h>

namespace BrainthreadIDE//::Debug
{
	class ThreadList
	{
	public:	
		struct Thread {
		  HANDLE Handle;
		  int Id;

		  unsigned int StepsExecuted;
		  bool Trace;
		  
		  unsigned int this_address;
		
		  Thread(int tid, HANDLE h) : Handle(h), Id(tid), this_address(0), StepsExecuted(-1), Trace(true){}
		};

	private:
		std::vector<Thread> threads;

	public:
		bool add_thread(Thread t)
		{
			 if(threads.size() < MAXIMUM_SUSPEND_COUNT)
			 {
				 threads.push_back(t);
				 return true;
			 }
			 return false;
		}

		bool remove_thread(int thread_id)
		{
			 for (std::vector<Thread>::iterator it = threads.begin(); it < threads.end(); ++it)
			 {
				 if(it->Id == thread_id)
				 {
					 threads.erase(it);
				     return true;
				 }
			 }
			 return false;
		}

		Thread* get_thread(int thread_id)
		{
			 for (std::vector<Thread>::iterator it = threads.begin(); it < threads.end(); ++it)
			 {
				 if(it->Id == thread_id)
					return &(*it);
			 }

			 return nullptr;
		}

		void set_block_threads(int runningThreadId, bool block)
		{
			 for (std::vector<Thread>::iterator it = threads.begin(); it < threads.end(); ++it)
			 {
				 if(block && it->Id != runningThreadId)
				 {
					 SuspendThread(it->Handle);
				 }
				 else if(block == false && it->Id != runningThreadId)
				 {
					 ResumeThread(it->Handle);
				 }
			 }
		}

	};
}