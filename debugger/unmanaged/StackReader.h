#pragma once

#include "MemoryReader.h"

class StackReader : public MemoryReader
{
    public:
		StackReader(HANDLE h, MemoryReader * cs_provider) : MemoryReader(h) 
		{
			this->cellsize_provider = cs_provider; 
		}

		~StackReader() {
		}

	private:
		MemoryReader * cellsize_provider;

    public:
		int ReadMemory(BTStackEntry &se)
		{
		    DWORD rv;
			unsigned int array_entrypoint;// = ((unsigned int)se.mem_pointer) + 0x7ed0;

			this->cell_size = cellsize_provider->SizeOfCell();

			this->alloc(se.size * this->cell_size);

			//read allocator entry
			ReadProcessMemory(hProcess,
				se.allocator,
				&array_entrypoint,
				sizeof(unsigned int), 
				&rv);	

			//read stack memory
			ReadProcessMemory(hProcess,
				(void *)array_entrypoint,
				this->memory,
				se.size * this->cell_size, 
				&rv);	
			
			return rv;
		}


};