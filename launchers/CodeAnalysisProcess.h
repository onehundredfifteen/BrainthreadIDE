#pragma once

#include "InterpreterProcess.h"

namespace BrainthreadIDE 
{
	using namespace System;

	public ref class CodeAnalysisProcess : public InterpreterProcess
	{
	public:
		CodeAnalysisProcess(bool run_selection) : InterpreterProcess(run_selection)
		{
			outputStrings = gcnew List<String ^>;
	        	
			worker->WorkerSupportsCancellation = false;

			if( this->GetType() == CodeAnalysisProcess::typeid)
			{ //other way doing this in RunCodeProcess 4fun
				worker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &CodeAnalysisProcess::worker_DoWork);
			}
			worker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &CodeAnalysisProcess::worker_RunWorkerCompleted);
		
			startInfo->Arguments = String::Format("-a {0} --verbose all --nopause",
												 this->GetCodeLocationArgument());

			startInfo->UseShellExecute = false;
			startInfo->CreateNoWindow = true;
			startInfo->RedirectStandardOutput = true;
			startInfo->RedirectStandardError = true;
		}

		virtual String ^ GetStatusLabel() override
		{
				return "Analysing code";
		}

		virtual bool Launch() override;
	    virtual void Stop() override;

	protected:
		void worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
		void worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e);

	private:
		void OutputHandler(System::Object^ sendingProcess,  DataReceivedEventArgs^ outLine);

    protected:
	    List<String ^> ^ outputStrings;	
	};
}


