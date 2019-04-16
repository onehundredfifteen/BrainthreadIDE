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

				//PAUSE only when option is set and is not a redirection
				if(true == GlobalOptions::Instance->PauseProgramAfterRun)
				{
					this->ForcedNoPause = false;
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

	protected:
		property bool ForcedNoPause
		{
			bool get()
			{
				return (this->startInfo && this->startInfo->Arguments->IndexOf(cNoPauseArg) > 0); //anyway cannot be first arg
			}
			void set(bool val)
			{
				if(nullptr == this->startInfo)
					throw gcnew Exception("Cannot force to pause a not working process"); 
				else if(true == GlobalOptions::Instance->CustomInterpreterFlag[ this->processWorkContext->settings->GetLanguage() ] )
					return; //not suport to custom interreters 	
				else if(true == val && false == this->ForcedNoPause)
					this->startInfo->Arguments = String::Concat( this->startInfo->Arguments->Trim(), " ", cNoPauseArg );
				else if(false == val && true == this->ForcedNoPause)
					this->startInfo->Arguments = startInfo->Arguments->Replace("--nopause", String::Empty);	
			}
		}

	private:	
		literal String ^ cNoPauseArg = "--nopause";
		
	};
}


