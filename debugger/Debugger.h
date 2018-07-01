#pragma once

#include "MemoryCellPrinter.h"
#include "ThreadResourceState.h"
#include "unmanaged/DebuggerLoop.h"

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
		Debugger(String ^ exe_path, String ^ args);
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
		void DetachDebugee();

		void LoadThreadTree(TreeView ^ threadTreeView);
		void LoadThreadList(ComboBox ^ threadListComboBox);
		void LoadStatus(Label ^ label);

		void LoadMemoryByThread();
		
		void LoadMemoryImageToView(DataGridView ^ dataGridView);
		void LoadMemoryImageToView(DataGridView ^ dataGridView, Int16 tab_cols);
		void LoadMemoryImageToView(DataGridView ^ dataGridView, MemoryCellPrinter ^ memPrinter);	

		void LoadMemoryImageToView(DataGridView ^ dataGridView, int threadId);

	property int CodePosition
		{
			int get() { 
				if(false == this->threadResource->ContainsKey(this->CurrentThreadId)) {
					return 0;
				}
				
				return this->threadResource[ this->CurrentThreadId ]->CodePosition; 
			}
		}
	property int CodePosition[int]
		{
			int get(int index) { 
				if(false == this->threadResource->ContainsKey(index)) {
					return 0;
				}
				
				return this->threadResource[ this->CurrentThreadId ]->CodePosition; 
			}
		}
	property int MemoryPosition
		{
			int get() { 
				if(false == this->threadResource->ContainsKey(this->CurrentThreadId)) {
					return 0;
				}
				
				return this->threadResource[ this->CurrentThreadId ]->MemoryPosition; 
			}
		}
	property int MemoryPosition[int]
		{
			int get(int index) { 
				if(false == this->threadResource->ContainsKey(index)) {
					return 0;
				}
				
				return this->threadResource[ this->CurrentThreadId ]->MemoryPosition; 
			}
		}
	property int MemoryLNZPos[int]
		{
			int get(int index) { 
				if(false == this->threadResource->ContainsKey(index)) {
					return 0;
				}
				
				return this->threadResource[ this->CurrentThreadId ]->MemoryLNZPos; 
			}
		}
	property bool ThreadTracing[int]
		{
			bool get(int index) { return this->threadResource[ index ]->Trace; }
			void set(int index, bool value) { 
				this->threadResource[ index ]->Trace = value; 
				this->updateTracing();
			}
		}

	property int CurrentThreadId
		{
			int get() { return this->getCurrentThreadId(); }
		}
    property int DebugeeProcessId
		{
			int get() { return this->debuggerLoop ? this->debuggerLoop->GetProcessId() : -1; }
		}
	property int DebugeeExitCode
		{
			int get() { return this->debuggerLoop ? this->debuggerLoop->GetProcessExitCode() : -1; }
		}

	property int MemoryForImage
		{
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

		Dictionary<int, ThreadResourceState ^> ^ threadResource;

		int memoryForImage;
		int memoryTableCols;
	
	private: 
		TreeNode ^ getThreadInfoTreeNode();
		TreeNode ^ getDetachedThreadInfoTreeNode(int threadId, String ^ format);
	
		array<int> ^ getMemoryImage(int last_cell_to_load);

		void customizeView(DataGridView ^ dataGridView);
		void updateTracing();
		bool compareValues(int a, int b, CompareType compare);
		String ^ getThreadName(int threadId);

		int getMemoryLen();
		int getCurrentThreadId(); 

	public: static String ^ DebuggerVersion()
		{
			return "0.2d";
		}
	};
}