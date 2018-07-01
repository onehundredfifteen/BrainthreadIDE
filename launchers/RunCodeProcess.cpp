#include "StdAfx.h"
#include "RunCodeProcess.h"

namespace BrainthreadIDE 
{
	bool RunCodeProcess::Launch()
	{
		if(worker->IsBusy)
			return false;

		this->OnStart(this, EventArgs::Empty);

		processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Running {0} with args: {1}", Path::GetFileName(startInfo->FileName),  
																											  startInfo->Arguments));
		
		worker->RunWorkerAsync();

		return true;
	}

	void RunCodeProcess::Stop()
	{
		worker->CancelAsync();

		this->OnComplete(this, EventArgs::Empty);
	}

    void RunCodeProcess::worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
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
			Thread::Sleep(115);   
		}
		
		e->Result = cli::safe_cast<int>( process->ExitCode );

		process->WaitForExit(); 
	}

	void RunCodeProcess::worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
	{
		if(e->Cancelled)    
		{
			processWorkContext->outputLister->AddOutputWithTimestamp("Program stopped by user");
		}
		else
		{
			if(false == GlobalOptions::Instance->PauseProgramAfterRun && process->HasExited)
				processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Program exited with code {0}", cli::safe_cast<int>(e->Result) ));
			else //program not exited
				processWorkContext->outputLister->AddOutputWithTimestamp("Execution completed");

			processWorkContext->outputLister->AddIDEOutput(this->GetProcessStatistics());
		}

		process->Close();

		this->OnComplete(this, EventArgs::Empty);
	}

	
	String ^ RunCodeProcess::GetProcessStatistics()
	{
		String ^ machineName = Environment::MachineName;

		try
		{
			if(process && process->HasExited == true)
			{
				if(String::Compare(machineName, process->MachineName) && String::Compare(".", process->MachineName))
					machineName = process->MachineName;
			
				return String::Format("@{0} {1} ms, CPU time: {2} ms", machineName, 
																		 (process->ExitTime - process->StartTime).TotalMilliseconds, 
																		 process->TotalProcessorTime.TotalMilliseconds);
			}
		}
		catch(InvalidOperationException ^ e)
		{
			return e->Message;
		}

		return "";
	}
}