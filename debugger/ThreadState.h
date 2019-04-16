#pragma once

#include <cstring>
#include "MemoryCellPrinter.h"
#include "unmanaged/btheaders/BTProcessMemoryInfo.h"

namespace BrainthreadIDE
{
    using namespace System;	
	using namespace System::Data;
	using namespace System::Collections::Generic;

	public ref class ThreadState
	{
	public:	
		ThreadState(const ThreadState ^ trs, const BTProcessProperties * snapshot, array<int> ^ mem_image)
			: trace(true), alive(true), threadId(trs->threadId)
		{
			this->processSnapshot = new BTProcessProperties;
			memcpy(this->processSnapshot, snapshot, sizeof(BTProcessProperties));

			this->memoryImage = mem_image;
			
			this->steps = this->processSnapshot->steps_executed;
			this->old_steps = trs->steps;
			this->entrypoint = trs->entrypoint;
			this->delta_steps = this->steps - this->old_steps;

		}

		ThreadState(const BTProcessProperties * snapshot, array<int> ^ mem_image)
			: trace(true), alive(true), threadId(snapshot->thread_id)
		{
			this->processSnapshot = new BTProcessProperties;
			memcpy(this->processSnapshot, snapshot, sizeof(BTProcessProperties));
			
			this->memoryImage = mem_image;

			this->steps = this->processSnapshot->steps_executed;
			this->old_steps = 1;
			this->entrypoint = this->CodePosition;
			this->delta_steps = 1;
		}

		ThreadState() 
			: trace(true), alive(true), threadId(-1)
		{
			this->memoryImage = gcnew array<int>(1);
			this->processSnapshot = new BTProcessProperties;
		}

		!ThreadState()
		{
			if(this->processSnapshot)
				delete this->processSnapshot;
		}
		~ThreadState()
		{
			this->!ThreadState();
		}

	public:

		property int CodePosition {
			int get() { 
				return this->processSnapshot->code_pointer_pos; 
			}
		}
		
		property int MemoryPosition {
			int get() { 
				return this->processSnapshot->meminfo.pointer_pos; 
			}
		}
		
		property int MemoryLNZPos {
			int get() { 
				return this->processSnapshot->meminfo.last_nonzero_cell; 
			}
		}

		property int Entrypoint {
			int get() { 
				return this->entrypoint; 
			}
		}

		property int ThreadId {
			int get() { 
				return threadId; 
			}
		}

		property bool Trace
		{
			bool get() { 
				return this->trace; 
			}
			void set(bool val) { 
				this->trace = val; 
			}
		}

		property bool Alive
		{
			bool get() { 
				return this->alive; 
			}
			void set(bool val) { 
				this->alive = val; 
			}
		}

		property int StepsExecuted {
			int get() { 
				return this->steps; 
			}
			void set(int val) { 
				this->steps = val; 
			}
		}

		property array<int> ^ MemoryImage
		{
			array<int> ^ get() { 
				return memoryImage; 
			}
		}

		

	public:
		String ^ Synthesis();

		array<String^> ^ MemorySynthesis();
		array<String^> ^ FunctionSynthesis(MemoryCellPrinter^ memPrinter);
		array<String^> ^ HeapSynthesis(MemoryCellPrinter^ memPrinter);
		array<String^> ^ SharedHeapSynthesis(MemoryCellPrinter^ memPrinter);


	private:	
		const int threadId;

		bool trace;
		bool alive;
		array<int> ^ memoryImage;
		BTProcessProperties * processSnapshot;

		/*int codePosition;
		int memoryPosition;
		int memoryLNZPos;*/

		int steps;
		int old_steps;
		int delta_steps;
		int entrypoint;

	private:
		array<String^>^ getHeapSynthesis(BTProcessStackInfo &si, MemoryCellPrinter^ memPrinter);
		void setGrayNodeCaptions()
		{
			/*for each(TreeNode ^ childNode in memoryInfoNode->Nodes)
			{
				childNode->ForeColor = System::Drawing::SystemColors::InactiveCaptionText;
				for each(TreeNode ^ _childNode in childNode->Nodes)
					_childNode->ForeColor = System::Drawing::SystemColors::InactiveCaptionText;
			}*/
		}

	};
}