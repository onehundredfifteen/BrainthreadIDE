#pragma once

#include "InterpreterProcess.h"

namespace BrainthreadIDE 
{
	using namespace System;

	public ref class RunCodeProcess : public InterpreterProcess
	{
	public:
		RunCodeProcess(bool run_selection) : InterpreterProcess(run_selection)
		{
			BrainthreadIDE::Language curLang = processWorkContext->settings->GetLanguage();

			if(GlobalOptions::Instance->CustomInterpreterFlag[ curLang ])
			{
				if(GlobalOptions::Instance->ReadLogFlag)
				{
					startInfo->Arguments = String::Format("{0} {1} {2}",
													 this->GetCodeLocationArgument(),
													 processWorkContext->settings->GetCustomRunString(),
													 GlobalOptions::Instance->LogPath);
				}
				else
				{
					startInfo->Arguments = String::Format("{0} {1}",
													 this->GetCodeLocationArgument(),
													 processWorkContext->settings->GetCustomRunString());
				}
			}
			else
			{
				if(GlobalOptions::Instance->ReadLogFlag)
				{
					startInfo->Arguments = String::Format("{0} -x {1} --log \"{2}\" --verbose important",
													 this->GetCodeLocationArgument(),
													 processWorkContext->settings->GetRunString(),
													 GlobalOptions::Instance->LogPath);
				}
				else
				{
					startInfo->Arguments = String::Format("{0} -x {1} --verbose important",
													 this->GetCodeLocationArgument(),
													 processWorkContext->settings->GetRunString());
				}

				//pause only when option is set and is not a redirection
				if(false == GlobalOptions::Instance->PauseProgramAfterRun 
					&& false == this->ProcessWorkContext->settings->GetRedirectionOption())
				{
					startInfo->Arguments = startInfo->Arguments + " --nopause";
				}	
			}

			worker->WorkerSupportsCancellation = true;

			startInfo->UseShellExecute = false;
			startInfo->CreateNoWindow = false;
			startInfo->RedirectStandardOutput = false;
			startInfo->RedirectStandardError = false;
		}

		virtual String ^ GetStatusLabel() override
		{
				return "Executing code";
		}


		virtual bool Launch() override;
	    virtual void Stop() override;

	protected:
		virtual void AttachWorkerEvents() override
		{
			worker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &RunCodeProcess::worker_DoWork);
			worker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &RunCodeProcess::worker_RunWorkerCompleted);
		}

	protected:
		String ^ GetProcessStatistics();

	private:
		void worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
		void worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);

	//protected:	
		//default log name
		//literal String ^ cLogFileName = "btrun.log";
		
	};
}


