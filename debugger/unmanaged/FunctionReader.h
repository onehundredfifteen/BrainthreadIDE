#pragma once

#include "StackReader.h"

class FunctionReader : public StackReader
{
    public:
		FunctionReader(HANDLE h, MemoryReader * cs_provider) : StackReader(h, cs_provider) 
		{
		}

		~FunctionReader() {
		}

    public:
		int ReadMemory(BTFunctionsEntry &fe)
		{
		    DWORD rv;
			unsigned int array_entrypoint;
			unsigned int lastitem_entrypoint;

			this->cell_size = (2 * sizeof(unsigned int)); //this stack is formed of std::pair<int,T>

			this->alloc(fe.callstack.size * this->cell_size);

			//read allocator entry
			ReadProcessMemory(hProcess,
				fe.callstack.allocator,
				&array_entrypoint,
				sizeof(unsigned int), 
				&rv);

			//read stack memory witout last element (discontinuous array)
			ReadProcessMemory(hProcess,
				(void *)array_entrypoint,
				this->memory,
				(fe.callstack.size ) * this->cell_size, 
				&rv);	

			//set last item pointer
			/*lastitem_entrypoint = array_entrypoint + sizeof(unsigned int);

			//read last item entry
			ReadProcessMemory(hProcess,
				(void *)lastitem_entrypoint,
			&this->memory[fe.callstack.size - 1],
				this->cell_size, 
				&rv);*/
			
			return rv;
		}

		int GetMemoryCellAt(unsigned int cell_index) //read first item of pair ->always int
		{
			if((int)cell_index >= this->GetRealMemorySize())
				return 0;
			
			return reinterpret_cast <int *>(this->memory)[2*cell_index];
		}

		int GetHiMemoryCellAt(unsigned int cell_index) //read second item of pair
		{
			if((int)cell_index >= this->GetRealMemorySize())
				return 0;

			int value = reinterpret_cast <int *>(this->memory)[2*cell_index+1];
			
			switch (cellsize_provider->SizeOfCell())
			{
				case 1:
					return (char)value;
				case 2:
					//value = value << 16;

					//return value >> 16;
					return (short)value;// >> 16;
				case 4:
				default:
					return value;
			}
		}


};