#pragma once

#include "BasicProcess.h"
#include "../options/GlobalOptions.h"

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Diagnostics;
	using namespace System::Threading;
	using namespace System::ComponentModel;

	public ref class InterpreterProcess abstract : public BasicProcess
	{
	public:
		InterpreterProcess(bool runSelection) : BasicProcess()
		{
			this->runSelection = runSelection;
			
			startInfo = gcnew ProcessStartInfo(GlobalOptions::Instance->InterpreterPath[ this->processWorkContext->settings->GetLanguage() ]);
		}

		virtual String ^ GetStatusLabel() = 0;

	protected:
		String ^ GetCodeLocationArgument();
		void ResolvePragmas();

	protected:
		Process ^ process;
		ProcessStartInfo ^ startInfo;
	
	private:
		bool runSelection;

	private:
		property String ^ Source
		{
			String ^ get()
			{
				if(runSelection == false)
					return processWorkContext->editorTextBox->richTextBox->Text;
				else
					return processWorkContext->editorTextBox->richTextBox->SelectedText;
			}
		}

	public:
		property WorkContext ^ ProcessWorkContext
		{
			WorkContext ^ get()
			{
				return processWorkContext;
			}
		}
	};
}
