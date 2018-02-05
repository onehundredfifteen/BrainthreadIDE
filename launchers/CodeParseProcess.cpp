#include "StdAfx.h"
#include "CodeParseProcess.h"

namespace BrainthreadIDE 
{
    void CodeParseProcess::worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
		try
		{
			process = gcnew Process;
			
			process->StartInfo = this->startInfo;
			process->OutputDataReceived += gcnew DataReceivedEventHandler(this, &CodeParseProcess::OutputHandler); 
			process->ErrorDataReceived  += gcnew DataReceivedEventHandler(this, &CodeParseProcess::OutputHandler);

			process->Start();
			process->BeginOutputReadLine();
			process->BeginErrorReadLine(); 
			process->WaitForExit();
		}
		catch(Exception ^ ex)
		{
			String ^msg = String::Format("Cannot launch a code parse process. Reason: {0}", ex->Message);
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

	void CodeParseProcess::OutputHandler(System::Object^ sendingProcess,  DataReceivedEventArgs^ outLine )
    {
		 if (!String::IsNullOrEmpty(outLine->Data))
		 { //same output as analyser, skipping some msgs
			if(outLine->Data->StartsWith(cWarning) == false && outLine->Data->Contains(cCodeSane) == false)
				outputStrings->Add(outLine->Data);

			if(outLine->Data->Contains(cCodeValid))
				code_is_valid = true;
		 }
	}
}
