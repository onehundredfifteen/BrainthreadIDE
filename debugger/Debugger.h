#pragma once

#include "ThreadMonitor.h"
//#include "ThreadState.h"
//#include "MemoryCellPrinter.h"
#include "unmanaged/DebuggerLoop.h"
#include "../options/Language.h"

namespace BrainthreadIDE 
{
	static BTProcessProperties ProcessProperties;

    using namespace System;	
	using namespace System::Data;
	using namespace System::Windows::Forms;
	using namespace System::Collections::Generic;

	public ref class Debugger
	{
	public:	
		Debugger(String ^ exe_path, String ^ args, BrainthreadIDE::Language lang);
		!Debugger();
		~Debugger();

	public:
		enum class CompareType {
				Equal,
				NotEqual,
				Bigger,
				Smaller
			};

		void RunStep();
		void RunToInstruction(int instruction_pos);
		void RunToMemoryTrap(int memory_index, int value, CompareType compare);

		void Finish();
		void DetachDebugee(bool kill_debugee);

		void LoadThreadTree(TreeView ^ threadTreeView);
		void LoadThreadList(ComboBox ^ threadListComboBox);
		void LoadStatus(Label ^ label);

		void UpdateDataFromMemory();
		void WriteMemoryToThread(String ^ value, int address, int threadId);
		
		void LoadMemoryImageToView(DataGridView ^ dataGridView);
		void LoadMemoryImageToView(DataGridView ^ dataGridView, Int16 tab_cols);
		void LoadMemoryImageToView(DataGridView ^ dataGridView, MemoryCellPrinter ^ memPrinter);	
		void LoadMemoryImageToView(DataGridView ^ dataGridView, int threadId);

		String ^ GetDebugStats();
		void ExceptionDumpToThreadTree(TreeView ^ threadTreeView);

	property int CodePosition {
			int get() { 
				return this->threadMonitor->CurrentThreadState()->CodePosition; 
			}
		}
	property int CodePosition[int] {
			int get(int index) { 
				if(false == this->threadMonitor->IsWatched(index)) {
					return 0;
				}
				
				return this->threadMonitor->GetThreadState(index)->CodePosition; 
			}
		}
	property int MemoryPosition {
			int get() { 
				return this->threadMonitor->CurrentThreadState()->MemoryPosition; 
			}
		}
	property int MemoryPosition[int] {
			int get(int index) { 
				if(false == this->threadMonitor->IsWatched(index)) {
					return 0;
				}
				
				return this->threadMonitor->GetThreadState(index)->MemoryPosition; 
			}
		}
	property int MemoryLNZPos[int]
		{
			int get(int index) { 
				if(false == this->threadMonitor->IsWatched(index)) {
					return 0;
				}
				
				return this->threadMonitor->GetThreadState(index)->MemoryLNZPos;
			}
		}
	property bool ThreadTracing[int]
		{
			bool get(int index) { 
				return this->threadMonitor->IsWatched(index) && this->threadMonitor->GetThreadState(index)->Trace; 
			}

			void set(int index, bool value) {
				ThreadState ^ state = this->threadMonitor->GetThreadState(index);

				if(state) {
					state->Trace = value; 
					this->updateThreadTracing();
				}
			}
		}

	property int CurrentThreadId {
			int get() { return this->threadMonitor->CurrentThreadId; }
		}
    property int DebugeeProcessId {
			int get() { return this->debuggerLoop ? this->debuggerLoop->GetProcessId() : -1; }
		}
	property int DebugeeExitCode {
			int get() { return this->debuggerLoop ? this->debuggerLoop->GetProcessExitCode() : -1; }
		}

	property int MemoryForImage {
			int get() { return memoryForImage; }
			void set(int value) 
			{ 
				if(value > memoryForImage)
					memoryForImage = value; 

				memoryForImage = Math::Min(this->memoryForImage, this->getMemoryLen());
			}
		}

	private:
		DebuggerLoop * debuggerLoop;
		MemoryCellPrinter ^ memPrinter;
		ThreadMonitor ^ threadMonitor;

		BrainthreadIDE::Language language;

		int memoryForImage;
		int memoryTableCols;
	
	private:
		array<int> ^ getMemoryImage(int last_cell_to_load);
		void customizeView(DataGridView ^ dataGridView);
		int getMemoryLen();

		void updateThreadTracing();
		void updateIdleThreadsProgress();

		bool compareValues(int a, int b, CompareType compare);

	public: static String ^ DebuggerVersion()
		{
			return "0.4";
		}

	/*private: 
		literal String ^ cThreadInfoNodeKey = "thread_info_node";*/
	};
}