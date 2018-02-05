#include "StdAfx.h"
#include "CodeAnalysisProcess.h"

namespace BrainthreadIDE 
{
	bool CodeAnalysisProcess::Launch()
	{
		if(Working())
			return false;

		this->OnStart(this, nullptr);
		
		processWorkContext->outputLister->AddOutputWithTimestamp("Task started");

		worker->RunWorkerAsync();

		return true;
	}

	void CodeAnalysisProcess::Stop()
	{
		return;
	}

    void CodeAnalysisProcess::worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
		try
		{
			process = gcnew Process;
			
			process->StartInfo = this->startInfo;
			process->OutputDataReceived += gcnew DataReceivedEventHandler(this, &CodeAnalysisProcess::OutputHandler); 
			process->ErrorDataReceived  += gcnew DataReceivedEventHandler(this, &CodeAnalysisProcess::OutputHandler);

			process->Start();
			process->BeginOutputReadLine();
			process->BeginErrorReadLine(); 
			process->WaitForExit();
		}
		catch(Exception ^ ex)
		{
			String ^msg = String::Format("Cannot launch a code analysis process. Reason: {0}", ex->Message);
			processWorkContext->outputLister->AddOutputWithTimestamp(msg);
			MessageBox::Show(msg, "Launch error", MessageBoxButtons::OK, MessageBoxIcon::Error);

			e->Result = cli::safe_cast<int>( -1 );
			process->Close(); 

			return;
		}

		//paranoic
		process->WaitForExit();

		e->Result = cli::safe_cast<int>( process->ExitCode );
		process->Close(); 
	}

	void CodeAnalysisProcess::worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
	{
		processWorkContext->outputLister->AddOutput(outputStrings);
		
		if(e->Error != nullptr /*|| cli::safe_cast<int>(e->Result) != 1*/)
		{
			processWorkContext->outputLister->AddOutputWithTimestamp("Code Analysis failed");
		}
		else
			processWorkContext->outputLister->AddOutputWithTimestamp("Task completed");

		this->OnComplete(this, nullptr);
	}

	void CodeAnalysisProcess::OutputHandler(System::Object^ sendingProcess,  DataReceivedEventArgs^ outLine )
    {
		 if (!String::IsNullOrEmpty(outLine->Data))
		 {
			outputStrings->Add(outLine->Data);
		 }
	}
}
