#pragma once

namespace BrainthreadIDE
{
	#define MEM_CHUNK_SIZE 1000
	#define ARRAY_MEM_SIZE 100

	struct BTProcessMemoryInfo
	{
		int memory_chunk[ MEM_CHUNK_SIZE ];

		unsigned int memory_len;
		unsigned int pointer_pos;

		int current_cell_value;
		int last_nonzero_cell;
		int cell_size;
	};

	typedef struct 
	{
		int array_memory[ ARRAY_MEM_SIZE ];

		int array_size;
	} BTProcessStackInfo, BTProcessThreadInfo;

	struct BTProcessProperties
	{
		BTProcessMemoryInfo meminfo;
		BTProcessStackInfo stackinfo;
		BTProcessStackInfo shstackinfo;
		BTProcessThreadInfo threadinfo;

		unsigned int code_pointer_pos;
		unsigned int steps_executed;
		unsigned int thread_id;
	};
}