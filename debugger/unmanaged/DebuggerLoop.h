#pragma once

#include "Breakpoint.h"
#include "Thread.h"
#include "FunctionReader.h"
#include "ThreadInfoReader.h"

#include "btheaders/BTProcessMemoryInfo.h"
#include "btheaders/DebugTimeException.h"

namespace BrainthreadIDE//::Debug
{
	#define DEBUG_BPCAT_FUN 1
	#define DEBUG_BPCAT_LOOP 2

	class DebuggerLoop
	{
	public:	
		DebuggerLoop(wchar_t * debugee, wchar_t * run_args);
		~DebuggerLoop();

		void Step(int cp);

		void TraceThread(int threadId, bool trace_this);
		bool IsThreadRunning(int threadId);
		int GetThreadSteps(int threadId);

		void Detach();
		void Terminate();

		void FillProcessProperties(BTProcessProperties &ppe);
		void FillProcessProperties(BTProcessProperties &ppe, int mem_chunk_offset);
		
		int GetProcessId();
		int GetProcessExitCode();
		bool WriteProcessMemory(int value, int index, int threadId);

		bool IsDebugeeRunning();

	private:
		HANDLE debugeeProcessHandle;
		PROCESS_INFORMATION procinfo;
		STARTUPINFO startupInfo;
		CONTEXT context;
		DEBUG_EVENT debug_event;
		SIZE_T rv;

	public:
		typedef bool Condition;

	private:
		BreakpointList bplist;
		ThreadList thlist;

		BTProcessProperties process_properties;
		
		Condition debugeeStarted;
		Condition readyToStepping;
		Condition useEcx;

	private:
		MemoryReader * memreader;
		ThreadInfoReader * thlreader;
		StackReader * heapreader;
		StackReader * shheapreader;
		FunctionReader * funreader;

	private:
		int HandleEvent();
		
		bool LoadSymbols(wchar_t * debugee);
		void WaitForFirstBreakpoint();

		unsigned int ReadCodePointerPosition();
		void ReadProcessProperties();

	};
}