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

		virtual bool Launch() override
		{
			this->OnStart(this, EventArgs::Empty);
			
			processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Running {0} separately with args: {1}", Path::GetFileName(startInfo->FileName),
																															 startInfo->Arguments));
		
			try
			{
				process = gcnew Process;
			
				process->StartInfo = this->startInfo;
				process->Start();
			}
			catch(Exception ^ ex)
			{
				String ^msg = String::Format("Cannot run the program. Reason: {0}", ex->Message);
				processWorkContext->outputLister->AddOutputWithTimestamp(msg);
				MessageBox::Show(msg, "Launch error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				process->Close();
			}

			this->OnComplete(this, EventArgs::Empty);

			return true;
		}
	};
}


