#pragma once

//#include <windows.h>

/*#include "MemoryTape.h"
#include "MemoryHeap.h"
#include "FunctionHeap.h"
#include "CodeTape.h"*/



#define CODEPOINTER_OFFSET (sizeof(void *) * 5)

struct BTMemoryTapeEntry
{
	void * pointer; //piórko

	void * mem; //pamiec
	unsigned len; //aktualny rozmiar pamieci

	void *max_mem; //ostatnia komórka pamiêci
}; 

struct BTStackEntry //for any std::stack or deque
{
	void * spare;
	void * mappointer;
	int mapsize;
	int myoff;
	int size;
};

struct BTThreadListEntry //for any std::vector;
{
	int allocator;
	void * first;
	void * last;
	void * end;
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

		T * pointer; //piórko

		T *mem; //pamiec
		unsigned len; //aktualny rozmiar pamieci

		T *max_mem; //ostatnia komórka pamiêci
		
		mem_option mem_behavior; //zachowanie pamieci
		eof_option eof_behavior; //reakcja na EOF z wejœcie	
};

*/