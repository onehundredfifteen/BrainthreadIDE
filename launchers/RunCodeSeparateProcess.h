#pragma once

#include "InterpreterProcess.h"

namespace BrainthreadIDE 
{
	using namespace System;
	
	public ref class RunCodeSeparateProcess : public InterpreterProcess
	{
	public:
		RunCodeSeparateProcess(bool run_selection) : InterpreterProcess(run_selection)
		{
			BrainthreadIDE::Language curLang = processWorkContext->settings->GetLanguage();
			
			if(GlobalOptions::Instance->CustomInterpreterFlag[ curLang ])
			{
				startInfo->Arguments = String::Format("{0} {1}",
													 this->GetCodeLocationArgument(),
													 processWorkContext->settings->GetCustomRunString());
			}
			else
			{

				startInfo->Arguments = String::Format("{0} -x {1} --verbose all",
													 this->GetCodeLocationArgument(),
													 processWorkContext->settings->GetRunString());
			}

			startInfo->UseShellExecute = false;
			startInfo->CreateNoWindow = false;
			startInfo->RedirectStandardOutput = false;
			startInfo->RedirectStandardError = false;
		}

		virtual String ^ GetStatusLabel() override
		{
				return "Launched program";
		}

		virtual bool Launch() override;
	    virtual void Stop() override;	
	};
}


