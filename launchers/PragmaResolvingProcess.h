#pragma once

#include "InterpreterProcess.h"

namespace BrainthreadIDE 
{
	using namespace System;
	
	public ref class PragmaResolvingProcess : public InterpreterProcess
	{
	public:
		PragmaResolvingProcess(bool run_selection) : InterpreterProcess(run_selection)
		{	
		}

		virtual String ^ GetStatusLabel() override
		{
			return "Resolving pragmas";
		}

		virtual bool Launch() override
		{
			this->OnStart(this, EventArgs::Empty);
			
			this->ResolvePragmas();

			this->OnComplete(this, EventArgs::Empty);
			return true;
		}	
		
	};
}


