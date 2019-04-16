#pragma once

namespace BrainthreadIDE
{
	#define MEM_CHUNK_SIZE 1000
	#define ARRAY_MEM_SIZE 64
	#define HALVE_INDEX 32 

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

	struct BTProcessFunctionInfo
	{
		int functions[ ARRAY_MEM_SIZE ];
		int return_addresses[ ARRAY_MEM_SIZE ];

		int callstack_size;
		int loaded_functions;
	};

	struct BTProcessProperties
	{
		BTProcessMemoryInfo meminfo;
		BTProcessStackInfo stackinfo;
		BTProcessStackInfo shstackinfo;
		BTProcessThreadInfo threadinfo;
		BTProcessFunctionInfo funinfo;

		unsigned int code_pointer_pos;
		unsigned int steps_executed;
		unsigned int thread_id;
	};
}