#include "StdAfx.h"

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
			this->processWorkContext->outputLister->AddOutputWithTimestamp("The debugging process can't be completed due to errors in the code.");
			MessageBox::Show("Your code contains errors.", "Cannot launch debugger", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);

			this->OnComplete(this, gcnew EventArgs);
			return;
		}

		processWorkContext->outputLister->AddIDEOutput(gcnew String('-', 30)); //chained with parser process
		processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Debugging with args: {0}", startInfo->Arguments));
		
		try
		{
			BrainthreadIDE::Language curLang = processWorkContext->settings->GetLanguage();
			
			this->debugger = gcnew BrainthreadIDE::Debugger(GlobalOptions::Instance->InterpreterPath[curLang]->Trim('"'), startInfo->Arguments->Trim('"'), curLang);
			this->process = Process::GetProcessById( this->debugger->DebugeeProcessId );
		
			//positioning debugee window
			this->MoveProcessWindow();
		}
		catch(Exception ^ ex) {
			this->processWorkContext->outputLister->AddOutputWithTimestamp("Cannot enter debug mode");
			this->processWorkContext->outputLister->AddIDEOutput(ex->Message);

			MessageBox::Show(ex->Message, "Cannot enter debug mode", MessageBoxButtons::OK, MessageBoxIcon::Error);
			this->OnComplete(this, gcnew EventArgs);
		}

		this->processWorkContext->outputLister->AddIDEOutput("Debugger attached to process");
		
		worker->RunWorkerAsync();
		//this->OnStart(this, gcnew EventArgs); //2nd call to refresh 1st step
	}

	void DebugCodeProcess::Stop()
	{
		worker->CancelAsync();
		debugger->Finish();	

		//this->OnComplete(this, EventArgs::Empty); //moved to worker_RunWorkerCompleted
	}

	void DebugCodeProcess::Detach()
	{
		worker->CancelAsync();
		debugger->DetachDebugee(false); 
		
		//this->OnComplete(this, EventArgs::Empty);
	}

    void DebugCodeProcess::worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
		while(this->process && !this->process->HasExited && this->debugger)
		{
			if(worker->CancellationPending) 
			{
				e->Cancel = true;
				break;
			}

			Thread::Sleep(115);   //maybe less?
		}
	}

	void DebugCodeProcess::worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
	{
		if(e->Cancelled)    
		{
			this->processWorkContext->outputLister->AddOutputWithTimestamp("Debug stopped by user");
		}
		else if(/*this == nullptr ||*/ this->debugger == nullptr) //event called to a dead pointer? kid of workaround
		{
			this->processWorkContext->outputLister->AddOutputWithTimestamp("Program exited prematurely");
			this->processWorkContext->outputLister->AddIDEOutput("Debugging terminated with unknown error. Cannot continue at this point.");

			this->OnComplete(this, gcnew EventArgs);
			return;
		}
		else if(debugger->DebugeeExitCode == -1)
		{
			this->processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Program exited with code {0}", debugger->DebugeeExitCode));
			this->processWorkContext->outputLister->AddIDEOutput("Debugging completed with error: Detached from debugee prematurely");

			try {
				debugger->DetachDebugee(true);
			}
			catch(Exception ^ ex) {
				this->processWorkContext->outputLister->AddIDEOutput(String::Format("Cannot kill debugee {0}. Try to restart IDE.", ex->Message));
			}
		}
		else
		{
			try {
				this->processWorkContext->outputLister->AddOutputWithTimestamp(String::Format("Program exited with code {0}", debugger->DebugeeExitCode));
				this->processWorkContext->outputLister->AddIDEOutput("Debugging completed");
				processWorkContext->outputLister->AddIDEOutput(debugger->GetDebugStats());
			}
			catch(...) {
				return;
			}
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
		catch(Exception ^ ex) {
			this->processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Step exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		
		CodeContextConverter ^ ccc = gcnew CodeContextConverter(processWorkContext);

		this->MoveCarretToPosition( ccc->ToCursor( this->debugger->CodePosition ));
	}

	void DebugCodeProcess::StepOver()
	{
		CodeContextConverter ^ ccc = gcnew CodeContextConverter(processWorkContext);
		int pi = CodeLoopBinder::FindEndOfLoop(this->debugger->CodePosition, ccc->RawSource);

		try
		{
			this->debugger->RunToInstruction( pi + 1 );
		}
		catch(Exception ^ ex) {
			this->processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Step over exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
		catch(Exception ^ ex) {
			this->processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
			MessageBox::Show(ex->Message, "Run to cursor exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}

		this->MoveCarretToPosition( ccc->ToCursor( this->debugger->CodePosition ));
	}

	void DebugCodeProcess::MemoryTrap(int index, int value, Debugger::CompareType compare)
	{
		try
		{
			this->debugger->RunToMemoryTrap(index - 1, value, compare);
		}
		catch(Exception ^ ex) {
			this->processWorkContext->outputLister->AddOutputWithTimestamp(ex->Message);
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

	bool DebugCodeProcess::PendingIO()
	{
		//check if program is waiting to input
		
		//not working well
		/*
		if(this->process && !this->process->HasExited)
		{
			for each(ProcessThread ^ th in this->process->Threads) {
				if(th->ThreadState == System::Diagnostics::ThreadState::Wait && th->WaitReason == ThreadWaitReason::UserRequest)
				{
					return true;
				}
			}
		}*/



		return false;
	}	
	
}