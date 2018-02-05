#pragma once

#include "../options/GlobalOptions.h"
#include "../WorkContexts.h"

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Diagnostics;
	using namespace System::Threading;
	using namespace System::ComponentModel;

	public ref class InterpreterProcess abstract
	{
	public:
		InterpreterProcess(bool runSelection);

		virtual bool Launch() = 0;
		virtual void Stop() = 0;
		bool Working();

		virtual String ^ GetStatusLabel();

	public:
		event System::EventHandler ^ OnStart;
		event System::EventHandler ^ OnComplete;

	protected:
		Process ^ process;
		ProcessStartInfo ^ startInfo;
		WorkContext ^ processWorkContext;
		System::ComponentModel::BackgroundWorker ^ worker;

	private:
		bool runSelection;

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

	protected:
		String ^ GetCodeLocationArgument();
	};
}
