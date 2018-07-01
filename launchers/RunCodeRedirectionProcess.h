#pragma once

#include "RunCodeProcess.h"
#include "../helpers/InputCharFormatter.h"

namespace BrainthreadIDE 
{
	using namespace System;
	
	public ref class RunCodeRedirectionProcess : public RunCodeProcess
	{
	public:
		RunCodeRedirectionProcess(String ^ input_str, bool run_selection) : RunCodeProcess(run_selection)
		{
			outputStrings = gcnew List<String ^>;

			this->input = InputCharFormatter::Format(input_str);

			startInfo->CreateNoWindow = true;
			startInfo->RedirectStandardOutput = true;
			startInfo->RedirectStandardError = true;
			startInfo->RedirectStandardInput = true;

			//dont affect this process ->flag is not set
			/*if(true == GlobalOptions::Instance->PauseProgramAfterRun) {
				startInfo->Arguments = startInfo->Arguments + " --nopause";
			}*/
		}

	protected:
		virtual void AttachWorkerEvents() override
		{
			worker->WorkerSupportsCancellation = true;
			worker->WorkerReportsProgress = true;

			worker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &RunCodeRedirectionProcess::worker_DoWork);
			worker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &RunCodeRedirectionProcess::worker_RunWorkerCompleted);
			worker->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &RunCodeRedirectionProcess::worker_ProgressChanged);
		}

	private:
		String ^input;
		List<String ^> ^ outputStrings;	

	private:
		void worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
		void worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);
		void worker_ProgressChanged(System::Object^ sender, System::ComponentModel::ProgressChangedEventArgs^ e);

		void OutputHandler(System::Object^ sendingProcess,  DataReceivedEventArgs^ outLine );
		void OutputErrorHandler(System::Object^ sendingProcess,  DataReceivedEventArgs^ outLine );
	};
}


