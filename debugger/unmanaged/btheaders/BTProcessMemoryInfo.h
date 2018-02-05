#pragma once

namespace BrainthreadIDE
{
	#define MEM_CHUNK_SIZE 1000
	#define LIST_MEM_SIZE 100

	struct BTProcessMemoryInfo
	{
		int memory_chunk[ MEM_CHUNK_SIZE ];

		unsigned int memory_len;
		unsigned int pointer_pos;

		int current_cell_value;
		int last_nonzero_cell;
	};

	typedef struct 
	{
		int list_memory[ LIST_MEM_SIZE ];

		unsigned int list_size;
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