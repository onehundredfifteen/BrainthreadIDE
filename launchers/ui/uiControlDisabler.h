#pragma once

#include "DelayedTask.h"

namespace BrainthreadIDE
{
    using namespace System;	

	public ref class uiControlDisabler : public DelayedTask
	{
	public:	
		uiControlDisabler(Control ^ _control,  int disable_time) : DelayedTask(disable_time)
		{
			this->control = _control;

			this->OnStart += gcnew System::EventHandler(this, &uiControlDisabler::DisableControl);
			this->OnComplete += gcnew System::EventHandler(this, &uiControlDisabler::EnableControl); 

			this->Launch();
		}

	private:
		Control ^ control;
	
	private:
		void EnableControl(System::Object ^, System::EventArgs ^)
		{
			control->Enabled = true;
		}
		void DisableControl(System::Object ^, System::EventArgs ^)
		{
			control->Enabled = false;
		}


	};

	
}