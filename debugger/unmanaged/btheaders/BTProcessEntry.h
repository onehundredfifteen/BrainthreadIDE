#pragma once

//memory fingerprints of data containers

#define CODEPOINTER_OFFSET (sizeof(void *) * 5)

struct BTMemoryTapeEntry //main memory
{
	void * pointer;
	void * mem; 
	unsigned len; 
	void *max_mem; 
}; 

struct BTStackEntry //for any std::stack or deque
{
	void * my_spare;
	void * allocator;
	int capacity;
	int my_offset;
	int size;
};

struct BTFunctionsEntry //for function heap object
{
	void * my_spare;
	void * my_spare_2;
	int map_size;
	int my_offset;

	BTStackEntry callstack;
};

struct BTThreadListEntry //for any std::vector;
{
	void * first;
	void * last;
	void * end;
	int my_val;
};

struct BTProcessEntry
{
	void * memory;
	void * heap;
	void * shared_heap;

	void * functions;

	void * code_tape;
	unsigned int code_pointer;

	BTThreadListEntry threadinfo;
};


/*
template < typename T >
class BrainThreadProcess
{
public:
	BrainThreadProcess(){};

	MemoryTape<T> *memory;
	MemoryHeap<T> *heap;
	MemoryHeap<T> *shared_heap;

	FunctionHeap<T> *functions;

	CodeTape * code;
	unsigned int code_pointer;
	
	std::vector< HANDLE >  child_threads;
};

template < typename T >
class MemoryTape
{
    public:
		typedef enum 
		{
			moLimited,
			moDynamic,
			moContinuousTape
		} mem_option;

		typedef enum
		{
			eoZero,
			eoMinusOne,
			eoUnchanged
		} eof_option;

		T * pointer; //pi�rko

		T *mem; //pamiec
		unsigned len; //aktualny rozmiar pamieci

		T *max_mem; //ostatnia kom�rka pami�ci
		
		mem_option mem_behavior; //zachowanie pamieci
		eof_option eof_behavior; //reakcja na EOF z wej�cie	
};

*/