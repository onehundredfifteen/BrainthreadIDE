#include "StdAfx.h"
#include "RunCodeSeparateProcess.h"

namespace BrainthreadIDE 
{
	bool RunCodeSeparateProcess::Launch()
	{
		processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Running {0} separately with args: {1}", Path::GetFileName(startInfo->FileName),
																														 startInfo->Arguments));
		
		try
		{
			process = gcnew Process;
			
			process->StartInfo = this->startInfo;
			process->Start();
		}
		catch(Exception ^ ex)
		{
			String ^msg = String::Format("Cannot run code. Reason: {0}", ex->Message);
			processWorkContext->outputLister->AddOutputWithTimestamp(msg);
			MessageBox::Show(msg, "Launch error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			process->Close();
		}

		return true;
	}

	void RunCodeSeparateProcess::Stop()
	{
		return;
	}
}