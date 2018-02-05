
#include "stdafx.h"
#include <vector>
#include <algorithm>

#include "Breakpoint.h"

namespace BrainthreadIDE//::Debug
{
	void BreakpointList::set_process(HANDLE h)
	{
		hProcess = h;
	}

	BreakpointList::Breakpoint* BreakpointList::get_breakpoint(unsigned int address)
	{
		for(std::vector<Breakpoint>::iterator it = breakpoints.begin(); it != breakpoints.end(); ++it)
		{
			if(it->real_address == address)
				return &(*it);
		}

		return nullptr;
	}

	void BreakpointList::add_breakpoint(unsigned int address, int cat_no)
	{
		breakpoints.push_back(BreakpointList::Breakpoint(address));
		breakpoint_categories[address] = cat_no;

		set_breakpoint(address);
	}

	void BreakpointList::add_breakpoint(unsigned int address)
	{
		add_breakpoint(address, 0);
	}

	void BreakpointList::set_breakpoint(unsigned int address)
	{
		SIZE_T rv;
		static char int3[] = { 0xcc };
		BreakpointList::Breakpoint * bp = get_breakpoint(address);
			  
		if (bp && bp->pending == false)
		{
				  ReadProcessMemory(hProcess,
							(void *)(bp->real_address),
							&(bp->real_byte),
							1, &rv);
				  WriteProcessMemory(hProcess,
							 (void *)(bp->real_address),
							 int3, 1, &rv);

				  FlushInstructionCache(hProcess, (void *)(bp->real_address), rv);

				  bp->pending = true;
		}
	}

	bool BreakpointList::unset_breakpoint(unsigned int address)
	{
		SIZE_T rv;
		Breakpoint * bp = get_breakpoint(address);
			  
		if (bp && bp->pending == true)
		{
				  bp->pending = false;
	
				  WriteProcessMemory(hProcess,
							 (void *)(bp->real_address),
							 &(bp->real_byte),
							 1, &rv);

				  FlushInstructionCache(hProcess, (void*)(bp->real_address), rv);

				  return true;
		}

		return false;
	}

	void BreakpointList::resume_breakpoint(unsigned int ss_address)
	{
			 std::vector<unsigned int> v;
			 std::vector<unsigned int>::iterator vit;

			 for(std::vector<Breakpoint>::iterator it = breakpoints.begin(); it != breakpoints.end(); ++it) 
				v.push_back(it->real_address); //get addresses

			 std::sort(v.rbegin(), v.rend()); //sort reverse

			 for(vit = v.begin(); vit != v.end(); ++vit)
			 {
				 if(ss_address > *vit)
					 break;
			 }

			 set_breakpoint(*vit);
	}

	bool BreakpointList::bp_in_category(unsigned int address, int cat_no)
	{
		std::map<unsigned int, int>::iterator it;

		it = breakpoint_categories.find(address);

		return it != breakpoint_categories.end() && 
			   cat_no == breakpoint_categories[address];
	}
}