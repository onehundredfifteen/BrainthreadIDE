#include "StdAfx.h"
#include "InterpreterProcess.h"
#include "../plugins/PragmaResolver.h"

namespace BrainthreadIDE 
{
	/*InterpreterProcess::InterpreterProcess(bool runSelection)
	{
		//worker = gcnew System::ComponentModel::BackgroundWorker();
		//worker->WorkerReportsProgress = false;
		//this->AttachWorkerEvents();

		this->runSelection = runSelection;
		//this->processWorkContext = WorkContextBroker::Instance->GetCurrentContext();

		startInfo = gcnew ProcessStartInfo(GlobalOptions::Instance->InterpreterPath[ this->processWorkContext->settings->GetLanguage() ]);
	}*/

	String ^ InterpreterProcess::GetCodeLocationArgument()
	{
		const int command_len_limit = 8000; 
		BrainthreadIDE::Language curLang = processWorkContext->settings->GetLanguage();

		//try resolve pragmas
		this->ResolvePragmas();

		//prepare source
		if(this->processWorkContext->fileContext 
		   && this->processWorkContext->fileContext->HasPhysicalFile() 
		   && GlobalOptions::Instance->SaveCodeBeforeRun == true
		   && runSelection == false)
		{ //source file
			//save code
			this->processWorkContext->fileContext->Content = this->Source;
			this->processWorkContext->fileContext->Save();
			
			if(GlobalOptions::Instance->CustomInterpreterFlag[curLang])
			{
				return String::Format("\"{0}\"", this->processWorkContext->fileContext->FilePath);
			}
			return String::Format("-s \"{0}\"", this->processWorkContext->fileContext->FilePath);
		}
		else if(this->Source->Length + this->processWorkContext->settings->GetAdditionalCommandsString()->Length > command_len_limit
			    || GlobalOptions::Instance->CustomInterpreterFlag[curLang] == true)
		{ // tmp source file
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
		else //raw source
		{
			return String::Format("-i \"{0}\"", this->Source);
		}
	}

	void InterpreterProcess::ResolvePragmas()
	{
		PragmaResolver ^ pragmaResolver = gcnew PragmaResolver(this->Source, GlobalOptions::Instance->Plugins);

		if(pragmaResolver->HasPragmas() /*&& this->runSelection == false*/) {
				pragmaResolver->Resolve(processWorkContext, this->processWorkContext->settings->GetLanguage());
		}
	}

}