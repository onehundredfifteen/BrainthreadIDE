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
			mainWindowLocation = mainwindow_location;
		}
		
		property Debugger^ DebuggerInstance
		{
			Debugger^ get() { return debugger; }
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

	protected:
		virtual void AttachWorkerEvents() override; 

	private:
		void worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
		void worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);

		void LaunchDebugger(System::Object^ sender, System::EventArgs^ e);
		void moveDebugeeWindow();

	private:	
		Debugger ^ debugger;
		System::Drawing::Rectangle mainWindowLocation;
		
	};
}


