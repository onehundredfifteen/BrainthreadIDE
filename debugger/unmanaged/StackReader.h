#pragma once

#include "MemoryReader.h"

class StackReader : MemoryReader
{
    public:
		StackReader() : MemoryReader() {
		}

		~StackReader() {
		}
/*
	protected:
		Cell * memory;
		int memory_len;
		int real_memory_len;
		int cell_size;

		HANDLE hProcess;
		*/

    public:
		int ReadMemory(BTStackEntry &se, short cell_size)
		{
		    DWORD rv;
			//unsigned int byte_memory_len = se.stack_size;

			//alloc(byte_memory_len + cell_size);
			//this->real_memory_len = this->memory_len * cell_size; 

			//alloc(100);

			/*ReadProcessMemory(hProcess,
				mte.mem,
				this->memory,
				this->real_memory_len, //mte.len * cell_size, 
				&rv);	
				*/

			return rv;
		}

		/*int GetMemoryCellAt(unsigned int rel_cell_address)
		{
			if((int)rel_cell_address >= this->real_memory_len)
				return 0;
			
			switch (cell_size)
			{
				case 1:
					return this->memory[rel_cell_address];
				case 2:
					return reinterpret_cast <short *>(this->memory)[rel_cell_address];
				case 4:
					return reinterpret_cast <int *>(this->memory)[rel_cell_address];
			}
			return 0;
		}

		int GetLastNZMemoryCell()
		{
			for(int i = memory_len - cell_size; i >= 0; i -= cell_size)
			{
				if(this->memory[i])
					return i / cell_size;
			}
			return -1;
		}*/


};