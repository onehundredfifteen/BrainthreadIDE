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
		virtual void ResolvePragmas();
		String ^ GetCodeLocationArgument();
		void MoveProcessWindow();

	protected:
		Process ^ process;
		ProcessStartInfo ^ startInfo;
		System::Drawing::Rectangle mainWindowLocation;
	
	private:
		bool runSelection;

	protected:
		property String ^ Source
		{
			String ^ get()
			{
				if(false == runSelection)
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
