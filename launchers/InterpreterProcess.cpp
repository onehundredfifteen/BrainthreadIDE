#include "StdAfx.h"
#include "InterpreterProcess.h"

namespace BrainthreadIDE 
{
	InterpreterProcess::InterpreterProcess(bool runSelection)
	{
		worker = gcnew System::ComponentModel::BackgroundWorker();
		worker->WorkerReportsProgress = false;

		this->runSelection = runSelection;
		processWorkContext = WorkContextBroker::Instance->GetCurrentContext();

		startInfo = gcnew ProcessStartInfo(GlobalOptions::Instance->InterpreterPath[ processWorkContext->settings->GetLanguage() ]);
	}

	String ^ InterpreterProcess::GetCodeLocationArgument()
	{
		const int command_len_limit = 8000; 
		BrainthreadIDE::Language curLang = processWorkContext->settings->GetLanguage();

		if(processWorkContext->fileContext 
		   && processWorkContext->fileContext->HasPhysicalFile() 
		   && GlobalOptions::Instance->SaveCodeBeforeRun == true
		   && runSelection == false)
		{
			//save code
			processWorkContext->fileContext->Content = this->Source;
			processWorkContext->fileContext->Save();
			
			if(GlobalOptions::Instance->CustomInterpreterFlag[curLang])
			{
				return String::Format("\"{0}\"", processWorkContext->fileContext->FilePath);
			}
			return String::Format("-s \"{0}\"", processWorkContext->fileContext->FilePath);
		}
		else if(this->Source->Length + processWorkContext->settings->GetAdditionalCommandsString()->Length > command_len_limit
			    || GlobalOptions::Instance->CustomInterpreterFlag[curLang] == true)
		{
			FileContext ^ tmpRunFile = gcnew FileContext(FileContext::BaseDirectory() + "\\__run.b");

			//throw if error?
			tmpRunFile->Content = this->Source;
			tmpRunFile->Save();

			if(GlobalOptions::Instance->CustomInterpreterFlag[curLang])
			{
				return String::Format("\"{0}\"", tmpRunFile->FilePath);
			}
			return String::Format("-s \"{0}\"", tmpRunFile->FilePath);
		}
		else
		{
			return String::Format("-i \"{0}\"", this->Source);
		}
	}

	bool InterpreterProcess::Working()
	{
		return worker->IsBusy;
	}

	String ^ InterpreterProcess::GetStatusLabel()
	{
		if(this->Working())
		{
			return "interpreter is running";
		}
		else
			return "interpreter is idle";
	}

}