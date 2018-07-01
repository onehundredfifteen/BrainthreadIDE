#pragma once

#include "../launchers/RunCodeRedirectionProcess.h"	

namespace BrainthreadIDE 
{
	public ref class RunProcessFactory 
	{
	public: static InterpreterProcess ^ createRunProcess(bool run_selection) 
			{
				WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
				
				if(currentWorkContext)
				{
					currentWorkContext->settings->Save();
					
					if(currentWorkContext->settings->GetRedirectionOption())
					{
						return gcnew RunCodeRedirectionProcess(currentWorkContext->settings->GetInputString(), run_selection);
					}
					else
						return gcnew RunCodeProcess(run_selection);
				}

				return nullptr;
			}
	};
}