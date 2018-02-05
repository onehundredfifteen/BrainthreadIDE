#pragma once

#include <stack>
#include <ostream>

/*
 * Klasa Stosu Pami�ci.
 * Pe�ni rol� pomocnicz� dla ta�my pami�ci. Dotatkowo pozwala zamienia�
 * ze sob� dwie ostatnie warto�ci. Wielko�c stosu ogranicza zmienna 'stack_limit'.
*/

template < typename T >
class MemoryHeap
{
	public:
		MemoryHeap(void){};
		~MemoryHeap(void){};

		void Push(T const&);
		T Pop(void);
		void Swap(void);

		std::ostream& PrintStack(std::ostream &s);

	protected:
		std::stack<T> mem_stack;
		T tmp;

		static const unsigned int stack_limit = 65536;
};