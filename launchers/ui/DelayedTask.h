#pragma once

#include "../BasicProcess.h"

namespace BrainthreadIDE 
{
	using namespace System;
	
	public ref class DelayedTask : public BasicProcess
	{
	public:
		DelayedTask(int interval) : delay_interval(interval), BasicProcess()
		{		
		}

		virtual bool Launch() override 
		{
			if(this->Working)
				return false;

			this->OnStart(this, nullptr);
			worker->RunWorkerAsync();

			return true;
		}

	private:
		int delay_interval;

	protected:
		virtual void AttachWorkerEvents() override
		{
			worker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &DelayedTask::worker_Sleep);
			worker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &DelayedTask::worker_RunWorkerCompleted);
		}

	private:
		void worker_Sleep(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
		{
			Thread::Sleep(delay_interval); 
		}
		void worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e)
		{
			this->OnComplete(this, nullptr);
		}
		
	};
}


