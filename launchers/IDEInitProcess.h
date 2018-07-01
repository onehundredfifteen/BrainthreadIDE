#pragma once

#include "BasicProcess.h"

namespace BrainthreadIDE 
{
	using namespace System;
	
	public ref class IDEInitProcess : public BasicProcess
	{
	public:
		IDEInitProcess() : BasicProcess()
		{		
		}

		virtual bool Launch() override;

	protected:
		virtual void AttachWorkerEvents() override
		{
			worker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &IDEInitProcess::worker_DoWork);
			worker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &IDEInitProcess::worker_RunWorkerCompleted);
		}

	private:
		void worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
		void worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);
		
	};
}


