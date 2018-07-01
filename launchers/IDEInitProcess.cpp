#include "StdAfx.h"
#include "IDEInitProcess.h"
#include "../helpers/BtOutputGenerator.h"

namespace BrainthreadIDE 
{
	bool IDEInitProcess::Launch()
	{
		if(this->Working)
			return false;

		this->OnStart(this, nullptr);
		
		//processWorkContext->outputLister->AddOutputWithTimestamp("Task started");

		worker->RunWorkerAsync();

		return true;
	}

    void IDEInitProcess::worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
		try
		{
			//BtOutputGenerator::BtOutputGenerator();
		}
		catch(...)
		{
			e->Result = false;
			return;
		}

		e->Result = true;
	}

	void IDEInitProcess::worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
	{
		//rocessWorkContext->outputLister->AddOutputWithTimestamp("Task completed");

		this->OnComplete(this, nullptr);
	}
}
