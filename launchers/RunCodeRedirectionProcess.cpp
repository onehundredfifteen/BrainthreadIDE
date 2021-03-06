#include "StdAfx.h"
#include "RunCodeRedirectionProcess.h"

namespace BrainthreadIDE 
{
	void RunCodeRedirectionProcess::worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
		try
		{
			process = gcnew Process;
			
			process->StartInfo = this->startInfo;
			process->ErrorDataReceived  += gcnew DataReceivedEventHandler(this, &RunCodeRedirectionProcess::OutputErrorHandler);
			process->OutputDataReceived  += gcnew DataReceivedEventHandler(this, &RunCodeRedirectionProcess::OutputHandler);
			process->EnableRaisingEvents = true;

			process->Start();
			process->BeginOutputReadLine();
			process->BeginErrorReadLine();

			StreamWriter^ myStreamWriter = process->StandardInput;
			if ( myStreamWriter )
			{ 
				myStreamWriter->WriteLine( this->input );
				myStreamWriter->Close();
			}

		}
		catch(Exception ^ ex)
		{
			String ^msg = String::Format("Cannot run the program. Reason: {0}", ex->Message);
			processWorkContext->outputLister->AddOutputWithTimestamp(msg);
			MessageBox::Show(msg, "Launch error", MessageBoxButtons::OK, MessageBoxIcon::Error);

			e->Result = cli::safe_cast<int>( -1 );
			process->Close();

			return;
		}
		
		while(!process->HasExited)
		{
			if(worker->CancellationPending) 
			{
				e->Cancel = true;

				try
				{
					process->CancelOutputRead();
					process->CancelErrorRead();
					process->Kill();
					process->WaitForExit();
				}
				catch(Exception ^ ex)
				{
					String ^msg = String::Format("Cannot cancel the process now. Reason: {0}", ex->Message);
					processWorkContext->outputLister->AddOutputWithTimestamp(msg);
					MessageBox::Show(msg, "Cancellation error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				break;
			}

			worker->ReportProgress(0);
			Thread::Sleep(115);   
		}
		
		//paranoic
		process->WaitForExit();

		e->Result = cli::safe_cast<int>( process->ExitCode ); 
	}

	void RunCodeRedirectionProcess::worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
	{
		processWorkContext->outputLister->AddOutput(outputStrings);
		
		if(e->Cancelled)    
		{
			processWorkContext->outputLister->AddOutputWithTimestamp("Program stopped by user");
		}
		else
		{
			processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Program exited with code {0}", cli::safe_cast<int>(e->Result) ));
		}

		processWorkContext->outputLister->AddIDEOutput(this->GetProcessStatistics());

		process->Close();
		this->OnComplete(this, EventArgs::Empty);
	}

	void RunCodeRedirectionProcess::worker_ProgressChanged( Object^ sender, ProgressChangedEventArgs^ e )
	{
		if(outputStrings->Count > 0)
		{
			processWorkContext->outputLister->AddOutput(outputStrings);
			outputStrings->Clear();
		}
	}

	void RunCodeRedirectionProcess::OutputHandler(System::Object^ sendingProcess,  DataReceivedEventArgs^ outLine )
    {
		if (!String::IsNullOrEmpty(outLine->Data))
		{
			outputStrings->Add(outLine->Data);
		}
	}

	void RunCodeRedirectionProcess::OutputErrorHandler(System::Object^ sendingProcess,  DataReceivedEventArgs^ outLine )
    {
		if (!String::IsNullOrEmpty(outLine->Data))
		{
			outputStrings->Add("[stderr] " + outLine->Data);
		}
	}
}