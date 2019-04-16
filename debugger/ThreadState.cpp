#include "stdafx.h"
#include <vcclr.h>

#include "ThreadState.h"

namespace BrainthreadIDE
{
	String ^ ThreadState::Synthesis()
	{
		if(false == this->trace)
			return String::Format("Steps: {0}", this->StepsExecuted);
		else if(false == this->alive)
			return String::Format("Exit at {0}, executed {1} steps", this->CodePosition, this->StepsExecuted);
		else if(this->delta_steps > 1)
			return String::Format("instruction {0}, steps: {1} (+{2})", 
											this->CodePosition, 
											this->StepsExecuted,
											this->delta_steps);
		else
			return String::Format("instruction {0}, steps: {1}", this->CodePosition, this->StepsExecuted);
	}
	
	/*
	* Syntesis are without a header
	*/
	array<String^> ^ ThreadState::MemorySynthesis()
	{
		array<String^>^ data = gcnew array<String^>(4);	
		float kb = (float)(this->processSnapshot->meminfo.memory_len * this->processSnapshot->meminfo.cell_size) / 1000.0F;

		array<MemoryCellPrinter^> ^ memPrinters = gcnew array<MemoryCellPrinter^>{gcnew MemoryCellPrinter(), gcnew CharacterCellPrinter(), gcnew HexCellPrinter()};
		
			
		data[0] = String::Format("Memory position {0}", this->processSnapshot->meminfo.pointer_pos + 1 );
		data[1] = String::Format("Current cell value: {0} '{1}' 0x{2}", memPrinters[0]->Print( this->processSnapshot->meminfo.current_cell_value ),
																			 memPrinters[1]->Print( this->processSnapshot->meminfo.current_cell_value ),
																			 memPrinters[2]->Print( this->processSnapshot->meminfo.current_cell_value ));

		data[2] =  String::Format("Last non-zero cell position: {0}", this->processSnapshot->meminfo.last_nonzero_cell+1);
		data[3] =  String::Format("Memory size: {0} ({1} kb)", this->processSnapshot->meminfo.memory_len, kb.ToString(L"F") );

		return data;
	}

	array<String^> ^ ThreadState::FunctionSynthesis(MemoryCellPrinter^ memPrinter)
	{
		List<String^>^ data = gcnew List<String^>(2);
		data->Add( String::Format("Callstack size: {0}", this->processSnapshot->funinfo.callstack_size ));

		//call stack elements
		for(int n = 1, i = Math::Min((int)this->processSnapshot->funinfo.callstack_size, (int)ARRAY_MEM_SIZE) - 1; i >= 0; --i, ++n)
			data->Add( String::Format("{1} @ {2}", n, 
										memPrinter->Print( this->processSnapshot->funinfo.functions[i] ),
										this->processSnapshot->funinfo.return_addresses[i],
										(n == 1 ? "Top" : "")));
		return data->ToArray();
	}

	array<String^> ^ ThreadState::getHeapSynthesis(BTProcessStackInfo &si, MemoryCellPrinter^ memPrinter)
	{
		List<String^>^ data = gcnew List<String^>(2);	
		data->Add( String::Format("Heap size: {0}", si.array_size ));

		//stack elements
		for(int n = 1, i = Math::Min((int)si.array_size, (int)ARRAY_MEM_SIZE) - 1; i >= 0; --i, ++n)
			data->Add( String::Format("{0}: {1} {2}", n, 
							memPrinter->Print( si.array_memory[i] ),
							(n == 1 ? "Top" : "")));

		return data->ToArray();
	}

	array<String^> ^ ThreadState::HeapSynthesis(MemoryCellPrinter^ memPrinter)
	{
		return this->getHeapSynthesis(this->processSnapshot->stackinfo, memPrinter);
	}

	array<String^> ^ ThreadState::SharedHeapSynthesis(MemoryCellPrinter^ memPrinter)
	{
		return this->getHeapSynthesis(this->processSnapshot->shstackinfo, memPrinter);
	}
}