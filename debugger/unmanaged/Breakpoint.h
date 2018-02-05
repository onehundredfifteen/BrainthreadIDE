#pragma once

#include <vector>
#include <map>
#include <windows.h>

namespace BrainthreadIDE//::Debug
{
	class BreakpointList
	{
	public:	
		struct Breakpoint {
		  bool pending;
		  unsigned char real_byte;
		  unsigned int real_address;

		  Breakpoint(unsigned int address) : pending(false), real_byte(0x0), real_address(address){}
		};


	private:
		std::vector<Breakpoint> breakpoints;
		std::map<unsigned int, int> breakpoint_categories;

		HANDLE hProcess;

	public:
		void set_process(HANDLE h);
		
		void add_breakpoint(unsigned int address);
		void add_breakpoint(unsigned int address, int cat_no);

		void set_breakpoint(unsigned int address);
		Breakpoint* get_breakpoint(unsigned int address);

		bool unset_breakpoint(unsigned int address);
		void resume_breakpoint(unsigned int ss_address);

		bool bp_in_category(unsigned int address, int cat_no);
	};
}