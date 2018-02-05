#pragma once

#include "MemoryReader.h"

class ThreadInfoReader : public MemoryReader
{
    public:
		ThreadInfoReader(HANDLE h) : MemoryReader(h) {
		}
		
		ThreadInfoReader() : MemoryReader() {
		}

    public:
		int ReadMemory(BTThreadListEntry &tle)
		{
		    DWORD rv;
			unsigned int byte_memory_len = (unsigned int)tle.last - (unsigned int)tle.first;
			cell_size = sizeof(HANDLE); //HANDLE
		
			if(byte_memory_len == 0)
			{
				this->memory_len = 0;
				return 0;
			}
			alloc(byte_memory_len);
			
			ReadProcessMemory(hProcess,
				tle.first,
				this->memory,
				byte_memory_len, 
				&rv);	

			return rv;
		}

		int GetMemoryCellAt(unsigned int cell_address)
		{
			if((int)cell_address >= this->GetRealMemorySize())
				return 0;
			
			return reinterpret_cast <int *>(this->memory)[cell_address];
		}


};