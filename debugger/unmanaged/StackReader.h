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

	protected:
		MemoryReader * cellsize_provider;

    public:
		int ReadMemory(BTStackEntry &se)
		{
		    DWORD rv;
			unsigned int * frames_entrypoint;

			if(se.size == 0)
				return 0;

			//calculate number of frames to read
			const int frame_size = se.capacity * 2;

			int frames_cnt = se.size / frame_size;
			if(se.size % frame_size) //ceil
				frames_cnt += 1;

			//alloc memory
			this->cell_size = cellsize_provider->SizeOfCell();

			

			this->alloc(frame_size * frames_cnt * this->cell_size);

			frames_entrypoint = new unsigned int[frames_cnt];

			//read allocator entry
			ReadProcessMemory(hProcess,
				se.allocator,
				frames_entrypoint,
				frames_cnt * sizeof(unsigned int), 
				&rv);	

			//read stack frames
			for(int i = 0; i < frames_cnt; ++i )
			{
				ReadProcessMemory(hProcess,
					(void *)frames_entrypoint[i],
					(this->memory + (i * frame_size * this->cell_size)),
					frame_size * this->cell_size, 
					&rv);	
			}

			delete [] frames_entrypoint;
			
			return rv;
		}

	private:
		int ReadAllocators(BTStackEntry &se, int &allocators_cnt, int * allocator_entrypoint)
		{
		    DWORD rv;
			

			//calculate number of frames in memory
			//if(se.size % se.capacity)
				//allocators_cnt += 1;

			

			//read allocator entry
			
			
			return rv;
		}
};