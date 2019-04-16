#pragma once

#include "MemoryReader.h"

class ThreadInfoReader : public MemoryReader
{
    public:
		ThreadInfoReader(HANDLE h) : MemoryReader(h) {
			this->cell_size = sizeof(HANDLE);
		}
		
		ThreadInfoReader() : MemoryReader() {
			this->cell_size = sizeof(HANDLE);
		}

    public:
		int ReadMemory(BTThreadListEntry &tle)
		{
		    DWORD rv;
			const unsigned int byte_memory_len = (unsigned int)tle.last - (unsigned int)tle.first;
			
			//this->cell_size = sizeof(HANDLE); //HANDLE
		
			if(byte_memory_len == 0)
			{
				this->memory_len = 0;
				return 0;
			}

			this->alloc(byte_memory_len);
			
			ReadProcessMemory(hProcess,
				tle.first,
				this->memory,
				byte_memory_len, 
				&rv);	

			return rv;
		}

		HANDLE GetMemoryCellAt(unsigned int cell_address)
		{
			if((int)cell_address >= this->GetRealMemorySize())
				return 0;
			
			return (HANDLE)((this->memory)[cell_address]);
		}


};