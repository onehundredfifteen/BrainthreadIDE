#pragma once

#include "CodeAnalysisProcess.h"

namespace BrainthreadIDE 
{
	using namespace System;

	public ref class CodeParseProcess : public CodeAnalysisProcess
	{
	public:
		CodeParseProcess(bool run_selection) : CodeAnalysisProcess(run_selection)
		{
			worker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &CodeParseProcess::worker_DoWork);
			code_is_valid = false;
		}

		CodeParseProcess(WorkContext ^ currentWorkContext, bool run_selection) : CodeAnalysisProcess(run_selection)
		{
			worker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &CodeParseProcess::worker_DoWork);
			code_is_valid = false;

			this->processWorkContext = currentWorkContext;
		}

		property bool ValidCode
		{
			bool get() { return code_is_valid; }
		}

		virtual String ^ GetStatusLabel() override
		{
			return "Parsing code";
		}

	protected:
		void worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
	
	private:
		void OutputHandler(System::Object^ sendingProcess,  DataReceivedEventArgs^ outLine);

	private:
		literal String ^ cError = "Error";
		literal String ^ cWarning = "Warning";
		literal String ^ cCodeValid = "Code is valid";
		literal String ^ cCodeSane = "Code is sane";

		bool code_is_valid;
	
	    
	};
}


