#include "stdafx.h"

#include <algorithm>
#include "DebuggerLoop.h"	

namespace BrainthreadIDE
{
	DebuggerLoop::DebuggerLoop(wchar_t * debugee, wchar_t * run_args)
	{	
		memset(&procinfo, 0, sizeof(procinfo));
		memset(&startupInfo, 0, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);

		context.ContextFlags = CONTEXT_FULL;

		if (!CreateProcess(debugee, run_args, 0, 0, 0,
							 CREATE_NEW_PROCESS_GROUP
							 | CREATE_SUSPENDED
							 | DEBUG_PROCESS
							 | DEBUG_ONLY_THIS_PROCESS,
							// | IDLE_PRIORITY_CLASS, 
							 0, 0, &startupInfo, &procinfo))
		{
			debugeeStarted = false;

			throw DebugTimeException( "Can't create process", GetLastError());
		}
		
		SetThreadPriority(procinfo.hThread, THREAD_PRIORITY_IDLE);

		debugeeStarted = true;
		readyToStepping = false;
		useEcx = false;

		debugeeProcessHandle = procinfo.hProcess;
		thlist.add_thread(ThreadList::Thread(procinfo.dwThreadId, procinfo.hThread));
		
		if(false == this->LoadSymbols(debugee)) {
			throw DebugTimeException( "Can't load symbols", GetLastError());
		}

		ResumeThread(procinfo.hThread);

		this->WaitForFirstBreakpoint();
		this->Step(0);
	}
	DebuggerLoop::~DebuggerLoop()
	{
		if(readyToStepping)
			CloseHandle(debugeeProcessHandle);

		if(debugeeStarted || this->memreader)
		{
			delete this->memreader;
			delete this->thlreader;
			delete this->heapreader;
			delete this->shheapreader;
		}
	}

	bool DebuggerLoop::LoadSymbols(wchar_t * debugee)
	{
		HANDLE hSymbolFile; 
		DWORD dwBytesRead = 0;
		BOOL res;

		const wchar_t p_ext[] = L".pdb";
		wchar_t symbolFilePath[MAX_PATH];
		unsigned int bp_addresses[12];
		unsigned int debug_flags[4];
		
		this->memreader = new MemoryReader(debugeeProcessHandle);
		this->thlreader = new ThreadInfoReader(debugeeProcessHandle);
		this->heapreader = new StackReader(debugeeProcessHandle, this->memreader);
		this->shheapreader = new StackReader(debugeeProcessHandle, this->memreader);

		//open and load file pdb
		wcscpy(symbolFilePath, debugee);
		wcscat(symbolFilePath, p_ext);
		hSymbolFile = CreateFile(symbolFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if(hSymbolFile == INVALID_HANDLE_VALUE)
			return false;
		
		res = ReadFile(hSymbolFile, bp_addresses, sizeof(bp_addresses), &dwBytesRead, NULL);
		if(FALSE == res || dwBytesRead != sizeof(bp_addresses)) {
			CloseHandle(hSymbolFile);
			return false;
		}

		//setting bpoints
		bplist.set_process(debugeeProcessHandle);

		for(int a = 0; a < sizeof(bp_addresses); a += 2)
		{
			if(bp_addresses[a] == 0x0)
				continue;
			
			bplist.add_breakpoint( bp_addresses[a], DEBUG_BPCAT_FUN);
			bplist.add_breakpoint( bp_addresses[a+1], DEBUG_BPCAT_LOOP);
		}

		//setting flags
		res = ReadFile(hSymbolFile, debug_flags, sizeof(debug_flags), &dwBytesRead, NULL);
		/*if(FALSE == res || dwBytesRead != sizeof(debug_flags)) {
			CloseHandle(hSymbolFile);
			return false;
		}*/
		useEcx = (debug_flags[0] == 0x73); // 0x73 is my flag value if set

		res = CloseHandle(hSymbolFile);

		return res == TRUE; //for c4805 warning
	}

	void DebuggerLoop::Step(int cp)
	{
		int contv = DBG_CONTINUE;
		ThreadList::Thread * curThread;

		ContinueDebugEvent(debug_event.dwProcessId, debug_event.dwThreadId, contv);

		while(readyToStepping && debugeeStarted)
		{
			debug_event.dwDebugEventCode = -1;
			WaitForDebugEvent(&debug_event, INFINITE);

			contv = HandleEvent();

			if(contv == DBG_CONTINUE &&
			   debug_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT &&
			   debug_event.u.Exception.ExceptionRecord.ExceptionCode == STATUS_BREAKPOINT &&
			   bplist.bp_in_category( (unsigned int)debug_event.u.Exception.ExceptionRecord.ExceptionAddress, DEBUG_BPCAT_LOOP))
			{
				//increase step number
				curThread = thlist.get_thread(debug_event.dwThreadId);
				curThread->StepsExecuted++;
				
				//read & refresh code_pointer_pos
				if(curThread->Trace && (cp == 0 || cp == ReadCodePointerPosition()))
				{
					ReadProcessProperties();
					break;
				}
			}
			else if(contv == DBG_EXCEPTION_NOT_HANDLED &&
				    debug_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) 
			{
				if(debug_event.u.Exception.dwFirstChance)
					throw DebugTimeException( "An internal first-chance exception was raised", debug_event.u.Exception.ExceptionRecord.ExceptionCode);
				else
					throw DebugTimeException( "An internal exception was raised", debug_event.u.Exception.ExceptionRecord.ExceptionCode);
			}
			else if(debug_event.dwDebugEventCode == RIP_EVENT)
			{
				throw DebugTimeException("A rip event was raised");
			}
			
			ContinueDebugEvent(debug_event.dwProcessId, debug_event.dwThreadId, contv);
		}
	}

	void DebuggerLoop::WaitForFirstBreakpoint()
	{
		int contv = DBG_CONTINUE;

		while(readyToStepping == false && debugeeStarted)
		{
			debug_event.dwDebugEventCode = -1;
			WaitForDebugEvent(&debug_event, INFINITE);

			contv = HandleEvent();

			if(contv == DBG_CONTINUE &&
			   debug_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT &&
			   debug_event.u.Exception.ExceptionRecord.ExceptionCode == STATUS_BREAKPOINT &&
			   bplist.bp_in_category( (unsigned int)debug_event.u.Exception.ExceptionRecord.ExceptionAddress, DEBUG_BPCAT_FUN)) 
			{
			     readyToStepping = true;
			}
			else if(contv == DBG_EXCEPTION_NOT_HANDLED &&
				    debug_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) 
			{
				if(debug_event.u.Exception.dwFirstChance)
					throw DebugTimeException( "An internal first-chance exception was raised", debug_event.u.Exception.ExceptionRecord.ExceptionCode);
				else
					throw DebugTimeException( "An internal exception was raised", debug_event.u.Exception.ExceptionRecord.ExceptionCode);
			}
			else if(debug_event.dwDebugEventCode == RIP_EVENT)
			{
				throw DebugTimeException("A rip event was raised");
			}
			else 
			{
				ContinueDebugEvent(debug_event.dwProcessId, debug_event.dwThreadId, contv);
			}

		}
	}

	unsigned int DebuggerLoop::ReadCodePointerPosition()
	{
		unsigned int cp;
		void * cp_address = (void *)(CODEPOINTER_OFFSET + thlist.get_thread(debug_event.dwThreadId)->this_address);
		
		ReadProcessMemory(debugeeProcessHandle,
		    cp_address,
		    &cp,
		    sizeof(unsigned int), 
			NULL);

		return cp;
	}

	void DebuggerLoop::ReadProcessProperties()
	{
		ThreadList::Thread * curThread;

		BTProcessEntry pe;
		BTMemoryTapeEntry mte;
		BTStackEntry se, shse;

		if(debugeeStarted == false || readyToStepping == false)
			return;

		curThread = thlist.get_thread(debug_event.dwThreadId);
		unsigned int entry_address = curThread->this_address;

		//zero meemory
		memset(&process_properties, 0, sizeof(process_properties));
		memset(&mte,  0, sizeof(BTMemoryTapeEntry));
		memset(&se,   0, sizeof(BTStackEntry));
		memset(&shse, 0, sizeof(BTStackEntry));
		memset(&pe,   0, sizeof(BTProcessEntry));

		//read bt process entry

		ReadProcessMemory(debugeeProcessHandle,
				(void *)entry_address,
				&pe,
				sizeof(BTProcessEntry), &rv);

		thlreader->ReadMemory(pe.threadinfo);

		//read brainfuck tape memory
		
		ReadProcessMemory(debugeeProcessHandle,
				pe.memory,
				&mte,
				sizeof(BTMemoryTapeEntry), &rv);

	    memreader->ReadMemory(mte);

		//read stack
		
		ReadProcessMemory(debugeeProcessHandle,
				pe.heap,
				&se,
				sizeof(BTStackEntry), &rv);

	    heapreader->ReadMemory(se);

		//read shared stack
		ReadProcessMemory(debugeeProcessHandle,
				pe.shared_heap,
				&shse,
				sizeof(BTStackEntry), &rv);

	    shheapreader->ReadMemory(shse);
		
		/*TEMPLATE
		void * a;
		ReadProcessMemory(debugeeProcessHandle,
				se.mappointer,
				&a,
				sizeof(void*), &rv);
				*/

		//fill structure
		process_properties.code_pointer_pos = pe.code_pointer;
		process_properties.steps_executed = curThread->StepsExecuted;
		process_properties.thread_id = curThread->Id;

		process_properties.meminfo.memory_len = mte.len; //memreader->GetRealMemorySize();
		process_properties.meminfo.pointer_pos = ((unsigned int)mte.pointer - (unsigned int)mte.mem) / memreader->SizeOfCell();
		process_properties.meminfo.current_cell_value = memreader->GetMemoryCellAt( process_properties.meminfo.pointer_pos );
		process_properties.meminfo.last_nonzero_cell = memreader->GetLastNZMemoryCell();
		process_properties.meminfo.cell_size = memreader->SizeOfCell();

		process_properties.stackinfo.array_size = se.size;
		process_properties.shstackinfo.array_size = shse.size;

		process_properties.threadinfo.array_size = 0;//TODO
	}

	void DebuggerLoop::GetProcessProperties(BTProcessProperties &ppe, int mem_chunk_offset)
	{
		//memory
		memset(&process_properties.meminfo.memory_chunk, 0, MEM_CHUNK_SIZE * sizeof(int));

		for(int i = 0; i < MEM_CHUNK_SIZE; ++i)
			process_properties.meminfo.memory_chunk[i] = this->memreader->GetMemoryCellAt(i + mem_chunk_offset);

		//thread list TODO
		//memset(&process_properties.threadinfo.array_memory, 0, ARRAY_MEM_SIZE * sizeof(int));
		
		/*for(int i = 0; i < ARRAY_MEM_SIZE; ++i)
		{
			int thandle = thlreader->GetMemoryCellAt(i);
			//if(thandle == 0)
				//break;

			process_properties.threadinfo.list_memory[ i ] = GetThreadId((HANDLE) thandle );
		}*/

		//stack
		memset(&process_properties.stackinfo.array_memory, 0, ARRAY_MEM_SIZE * sizeof(int));
		
		for(int i = 0; i < process_properties.stackinfo.array_size && i < ARRAY_MEM_SIZE; ++i)
			process_properties.stackinfo.array_memory[i] = this->heapreader->GetMemoryCellAt(i);

		//shared stack
		memset(&process_properties.shstackinfo.array_memory, 0, ARRAY_MEM_SIZE * sizeof(int));
		
		for(int i = 0; i < process_properties.shstackinfo.array_size && i < ARRAY_MEM_SIZE; ++i)
			process_properties.shstackinfo.array_memory[i] = this->shheapreader->GetMemoryCellAt(i);

		//return
		ppe = process_properties;
	}

	void DebuggerLoop::GetProcessProperties(BTProcessProperties &ppe)
	{
		GetProcessProperties(ppe, 0);
	}

	bool DebuggerLoop::WriteProcessMemory(int value, int index, int threadId)
	{
		SIZE_T rv;
		BTProcessEntry pe;
		BTMemoryTapeEntry mte;
		memset(&pe, 0, sizeof(pe));
		memset(&mte,  0, sizeof(BTMemoryTapeEntry));

		ThreadList::Thread * curThread;
		unsigned int entry_address, edit_address;
		
		if(debugeeStarted == false || readyToStepping == false)
			return false;

		curThread = thlist.get_thread(threadId);
		entry_address = curThread->this_address;

		//read bt process entry
		::ReadProcessMemory(debugeeProcessHandle,
				(void *)entry_address,
				&pe,
				sizeof(BTProcessEntry), 
				&rv);

		::ReadProcessMemory(debugeeProcessHandle,
				pe.memory,
				&mte,
				sizeof(BTMemoryTapeEntry), 
				&rv);

		//set address
		edit_address = ((unsigned int)mte.mem) + (index * this->memreader->SizeOfCell());
		
		//write new value
		::WriteProcessMemory(debugeeProcessHandle,
				(void *)edit_address,
				&value,
				this->memreader->SizeOfCell(), 
				&rv);

	    return rv == this->memreader->SizeOfCell();
	}

	int DebuggerLoop::HandleEvent()
	{
        int contv = DBG_CONTINUE;
		HANDLE hThread = (HANDLE)0L;

		switch (debug_event.dwDebugEventCode)
		{
			case CREATE_PROCESS_DEBUG_EVENT:
			case LOAD_DLL_DEBUG_EVENT:
			case UNLOAD_DLL_DEBUG_EVENT:
			case OUTPUT_DEBUG_STRING_EVENT:
				break;

			case CREATE_THREAD_DEBUG_EVENT:

				thlist.add_thread(ThreadList::Thread(debug_event.dwThreadId, debug_event.u.CreateThread.hThread));
				break;

			case EXIT_THREAD_DEBUG_EVENT:

			    thlist.remove_thread(debug_event.dwThreadId);
				break;

			case EXCEPTION_DEBUG_EVENT:
				
				hThread = thlist.get_thread(debug_event.dwThreadId)->Handle;
				rv = GetThreadContext(hThread, &context);

			    switch (debug_event.u.Exception.ExceptionRecord.ExceptionCode)
				{
					case STATUS_BREAKPOINT:
					
						if (bplist.unset_breakpoint((unsigned int)debug_event.u.Exception.ExceptionRecord.ExceptionAddress))
						{
							context.Eip --;
							if (rv != -1)
							{			
								//if(debug_event.u.Exception.ExceptionRecord.ExceptionAddress == (void*)DEBUG_BPADDR_FUN)
								if(bplist.bp_in_category( (unsigned int)debug_event.u.Exception.ExceptionRecord.ExceptionAddress, DEBUG_BPCAT_FUN))
								{
									//thread that meet a 1st bp 
									thlist.get_thread(debug_event.dwThreadId)->this_address = useEcx ? context.Ecx : context.Edi;
								}
							
								if ((context.EFlags & 0x100) == false) //set single stepping flag for resume bp
								{
									context.EFlags |= 0x100; 
								}

								SetThreadContext(hThread, &context);

								thlist.set_block_threads(debug_event.dwThreadId, true);
							}
		 
						}
						break;

					case STATUS_SINGLE_STEP:
					    
					    bplist.resume_breakpoint((unsigned int)debug_event.u.Exception.ExceptionRecord.ExceptionAddress);
				 
						if ((context.EFlags & 0x100)) //unset single step flag
						{
							 context.EFlags &= ~0x100;
							 SetThreadContext(hThread, &context);
						}

						thlist.set_block_threads(debug_event.dwThreadId, false);
				  
				        break;

				    default:
				   
					   contv = DBG_EXCEPTION_NOT_HANDLED;

					   break;
				}

				break;

			case EXIT_PROCESS_DEBUG_EVENT:
			case RIP_EVENT:

	           debugeeStarted = false;//to kill loop
			   break;
		}

	    return contv;	
	}

	void DebuggerLoop::TraceThread(int threadId, bool trace_this)
	{
		ThreadList::Thread * curThread = thlist.get_thread(threadId);
		
		if(curThread)
			curThread->Trace = trace_this;
	}

	int DebuggerLoop::GetThreadSteps(int threadId)
	{
		ThreadList::Thread * curThread = thlist.get_thread(threadId);
		
		if(curThread)
			return curThread->StepsExecuted;

		return -1;
	}

	bool DebuggerLoop::IsThreadRunning(int threadId)
	{
		return thlist.get_thread(threadId) != nullptr;
	}

	void DebuggerLoop::Terminate()
	{	
		this->Detach();
		TerminateProcess(debugeeProcessHandle, 0);
	}

	void DebuggerLoop::Detach()
	{
		if(IsDebugeeRunning())	
		{
			ContinueDebugEvent(debug_event.dwProcessId, debug_event.dwThreadId, DBG_CONTINUE);
			DebugActiveProcessStop(procinfo.dwProcessId);
		}
	}

	int DebuggerLoop::GetProcessId()
	{
		return procinfo.dwProcessId;
	}

	int DebuggerLoop::GetProcessExitCode()
	{
		DWORD exit_code;
		if(false == IsDebugeeRunning() && GetExitCodeProcess(debugeeProcessHandle, &exit_code))
		{
			return exit_code;
		}
		return -1;
	}

	bool DebuggerLoop::IsDebugeeRunning()
	{
		HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, procinfo.dwProcessId);
		DWORD ret = WaitForSingleObject(process, 0);
		CloseHandle(process);

		return ret == WAIT_TIMEOUT;
	}
}