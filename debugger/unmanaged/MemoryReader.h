#pragma once

#include "btheaders/BTProcessEntry.h"

class MemoryReader
{
    public:
		typedef BYTE Cell, *pCell;
		
		MemoryReader(HANDLE h) {
			this->memory = nullptr;
			this->memory_len = -1;
			this->set_process(h);
		}

		MemoryReader() {
			this->memory = nullptr;
			this->memory_len = -1;
		}

		~MemoryReader() {
			if(this->memory)
				delete [] this->memory;
		}

		void set_process(HANDLE h) {
			hProcess = h;
			cell_size = sizeof(Cell);
		}

	protected:
		Cell * memory;
		int memory_len;
		//int real_memory_len;
		int cell_size;

		HANDLE hProcess;

	protected:
		void alloc(int new_memory_len)
		{	  
			if(this->memory_len != new_memory_len)
			{
				if(this->memory)
					delete [] this->memory;
				
				this->memory = new Cell [new_memory_len];
				this->memory_len = new_memory_len;
				//this->real_memory_len = this->memory_len * cell_size;

				memset(this->memory, 0, sizeof(Cell) * this->memory_len);
			}
		}

    public:
		int ReadMemory(BTMemoryTapeEntry &mte)
		{
		    DWORD rv;
			unsigned int byte_memory_len = (unsigned int)mte.max_mem - (unsigned int)mte.mem;

			if(mte.len == 1 && byte_memory_len == 0)
				cell_size = 1;
			else
				cell_size = byte_memory_len / (mte.len - 1);

			alloc(mte.len * cell_size);
			 
			ReadProcessMemory(hProcess,
				mte.mem,
				this->memory,
				mte.len * cell_size, 
				&rv);	

			return rv;
		}

		int GetMemoryCellAt(unsigned int cell_address)
		{
			if((int)cell_address >= this->GetRealMemorySize())
				return 0;
			
			switch (cell_size)
			{
				case 1:
					return this->memory[cell_address];
				case 2:
					return reinterpret_cast <short *>(this->memory)[cell_address];
				case 4:
					return reinterpret_cast <int *>(this->memory)[cell_address];
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
		}

		int GetRealMemorySize()
		{
			return this->memory_len / cell_size;
		}


};