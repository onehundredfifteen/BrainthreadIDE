#include "StdAfx.h"

#pragma comment(lib, "User32.lib")

#include "DebugCodeProcess.h"
#include "CodeParseProcess.h"
#include "../helpers/CodeContextConverter.h"
#include "../helpers/CodeLoopBinder.h"

namespace BrainthreadIDE 
{
	bool DebugCodeProcess::Launch()
	{
		if(this->Working)
			return false;

		this->OnStart(this, EventArgs::Empty);

		CodeParseProcess ^ parseProcess = gcnew CodeParseProcess(this->processWorkContext, false);

		//chain launch debugger -> launch after successfull parse
		parseProcess->OnComplete += gcnew System::EventHandler(this, &DebugCodeProcess::LaunchDebugger); //!

		if(false == parseProcess->Launch())
		{
			processWorkContext->outputLister->AddOutputWithTimestamp("The parser cannot be started.");
			processWorkContext->outputLister->AddIDEOutput("The debugging process can't be completed due to the pending task.");
			MessageBox::Show("The parser cannot be started", "Terminating", MessageBoxButtons::OK, MessageBoxIcon::Error);
			
			this->OnComplete(this, gcnew EventArgs);
			return false;
		}
	 
		return true;
	}

	void DebugCodeProcess::LaunchDebugger(System::Object^  sender, System::EventArgs^  e)
	{
		//processWorkContext->outputLister->Clear();
		if(cli::safe_cast<CodeParseProcess^ >(sender)->ValidCode == false)
		{
			processWorkContext->outputLister->AddOutputWithTimestamp("The debugging process can't be completed due to errors in the code.");
			MessageBox::Show("Your code contains errors.", "Cannot launch debugger", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);

			this->OnComplete(this, gcnew EventArgs);
			return;
		}

		processWorkContext->outputLister->AddIDEOutput(gcnew String('-', 30)); //chained with parser process
		processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Debugging with args: {0}", startInfo->Arguments));
		
		try
		{
			BrainthreadIDE::Language curLang = processWorkContext->settings->GetLanguage();
			
			debugger = gcnew BrainthreadIDE::Debugger(GlobalOptions::Instance->InterpreterPath[curLang]->Trim('"'), startInfo->Arguments->Trim('"'));
			process = Process::GetProcessById( debugger->DebugeeProcessId );
		
			//positioning debugee window
			this->moveDebugeeWindow();
		}
		catch(ArgumentException ^ ex)
		{
			processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Cannot launch debugger", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch(Exception ^ ex)
		{
			processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Cannot enter debug mode", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}

		processWorkContext->outputLister->AddIDEOutput(String::Format("Debugger attached to process"));
		
		worker->RunWorkerAsync();
	}

	void DebugCodeProcess::Stop()
	{
		worker->CancelAsync();
		debugger->Finish();	

		this->OnComplete(this, EventArgs::Empty);
	}

	void DebugCodeProcess::Detach()
	{
		worker->CancelAsync();
		debugger->DetachDebugee(); 
		
		this->OnComplete(this, EventArgs::Empty);
	}

    void DebugCodeProcess::worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
		while(process && !process->HasExited)
		{
			if(worker->CancellationPending) 
			{
				e->Cancel = true;
				break;
			}
			Thread::Sleep(115);   
		}
	}

	void DebugCodeProcess::worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
	{
		if(e->Cancelled)    
		{
			processWorkContext->outputLister->AddOutputWithTimestamp("Debug stopped by user");
		}
		else
		{
			processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Program exited with code {0}", debugger->DebugeeExitCode));
			processWorkContext->outputLister->AddIDEOutput("Debugging completed");
		}

		this->OnComplete(this, EventArgs::Empty);
	}

	//special debugger methods
	void DebugCodeProcess::Step()
	{
		try
		{
			this->debugger->RunStep();
		}
		catch(Exception ^ ex)
		{
			processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Step exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		
		CodeContextConverter ^ ccc = gcnew CodeContextConverter(processWorkContext);

		this->MoveCarretToPosition( ccc->ToCursor( this->debugger->CodePosition ));
	}

	void DebugCodeProcess::StepOver()
	{
		CodeContextConverter ^ ccc = gcnew CodeContextConverter(processWorkContext);
		int pi = CodeLoopBinder::GetPairedInstruction(this->debugger->CodePosition, ccc->RawSource);

		try
		{
			this->debugger->RunToInstruction( pi + 1 );
		}
		catch(Exception ^ ex)
		{
			processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Step exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}

		this->MoveCarretToPosition( ccc->ToCursor( this->debugger->CodePosition ));
	}

	void DebugCodeProcess::RunToCursor()
	{
		CodeContextConverter ^ ccc = gcnew CodeContextConverter( processWorkContext);
		
		try
		{
			this->debugger->RunToInstruction( ccc->ToInstruction( processWorkContext->editorTextBox->richTextBox->SelectionStart ));
		}
		catch(Exception ^ ex)
		{
			processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Step exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}

		this->MoveCarretToPosition( ccc->ToCursor( this->debugger->CodePosition ));
	}

	void DebugCodeProcess::MemoryTrap(int index, int value, Debugger::CompareType compare)
	{
		try
		{
			this->debugger->RunToMemoryTrap(index - 1, value, compare);
		}
		catch(Exception ^ ex)
		{
			processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Memory Trap exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}

		CodeContextConverter ^ ccc = gcnew CodeContextConverter(processWorkContext);
		this->MoveCarretToPosition( ccc->ToCursor( this->debugger->CodePosition ));
	}

	void DebugCodeProcess::MoveCarretToPosition(int new_pos)
	{	
		RichTextBox ^ richTextBox = processWorkContext->editorTextBox->richTextBox;
		int cur_pos = richTextBox->SelectionStart;

		richTextBox->SelectionStart = new_pos;

		if(GlobalOptions::Instance->DebugStepSelect)
			richTextBox->SelectionLength = 1;
		
		if(richTextBox->GetLineFromCharIndex(cur_pos) != richTextBox->GetLineFromCharIndex(new_pos))
		{ //prevent blinking
			richTextBox->ScrollToCaret();
		}
		
		richTextBox->Focus();
	}

	void DebugCodeProcess::moveDebugeeWindow()
	{
		int x, y;
		int flags = SWP_NOSIZE; // Ignores size arguments. 
		HWND hia = GlobalOptions::Instance->DebugeeWindowStyle == 2 ? HWND_TOPMOST :  HWND_TOP;

		if(GlobalOptions::Instance->DebugeeWindowStyle == 1)
			flags |= SWP_HIDEWINDOW;

		switch(GlobalOptions::Instance->DebugeeWindowPosition)
		{
		case 1: //quasi center
			x = mainWindowLocation.X + mainWindowLocation.Width / 3;
			y = mainWindowLocation.Y + mainWindowLocation.Height / 3;
			break;
		case 2: //top left
			x = 0;
			y = 0;
			break;
		case 3: //align right
			x = mainWindowLocation.Right;
			y = mainWindowLocation.Y;
			break;
		default:
		case 0: //default
			flags |= SWP_NOMOVE; break;

		}

		::SetWindowPos((HWND)process->MainWindowHandle.ToPointer(), hia, x, y, 0, 0, flags); 
	}

	
	
}