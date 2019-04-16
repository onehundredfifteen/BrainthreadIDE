#pragma once

#include "InterpreterProcess.h"
#include "../plugins/PragmaResolver.h"

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

		virtual void ResolvePragmas() override
		{
			PragmaResolver ^ pragmaResolver = gcnew PragmaResolver(this->Source, GlobalOptions::Instance->Plugins);

			if(pragmaResolver->HasPragmas()) {
					pragmaResolver->Resolve(processWorkContext, this->processWorkContext->settings->GetLanguage());
			}
			else
				this->processWorkContext->outputLister->AddIDEOutput("No pragma found in the code");
		}
		
	};
}


