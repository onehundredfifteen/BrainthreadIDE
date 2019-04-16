#pragma once

#include "RunCodeProcess.h"
#include "../debugger/Debugger.h"

namespace BrainthreadIDE 
{
	using namespace System;

	public ref class DebugCodeProcess : public RunCodeProcess
	{
	public:
		DebugCodeProcess(System::Drawing::Rectangle mainwindow_location, bool run_selection) : RunCodeProcess(run_selection)
		{
			BrainthreadIDE::Language curLang = processWorkContext->settings->GetLanguage();
			this->mainWindowLocation = mainwindow_location;

			if(false == this->ForcedNoPause)
			{
				//startInfo->Arguments = startInfo->Arguments->Replace("--nopause", String::Empty);
				this->ForcedNoPause = true;
			}	
		}
		
		property Debugger^ DebuggerInstance
		{
			Debugger^ get() { return this->debugger; }
		}

		virtual String ^ GetStatusLabel() override
		{
			return "Debugging code";
		}

		virtual bool Launch() override;
	    virtual void Stop() override;
		void Detach();

	public:
		void Step();
		void StepOver();
		void RunToCursor();
		void MemoryTrap(int index, int value, Debugger::CompareType compare);

		void MoveCarretToPosition(int new_pos);
		bool PendingIO();

	protected:
		virtual void AttachWorkerEvents() override
		{
			worker->WorkerSupportsCancellation = true;
			
			worker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &DebugCodeProcess::worker_DoWork);
			worker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &DebugCodeProcess::worker_RunWorkerCompleted);
		}

	private:
		void worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
		void worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);

		void LaunchDebugger(System::Object^ sender, System::EventArgs^ e);

	private:	
		Debugger ^ debugger;
		
	};
}


