#include "StdAfx.h"

#pragma comment(lib, "User32.lib")
#include <windows.h> //for move process window

#include "InterpreterProcess.h"
#include "../plugins/PragmaResolver.h"

namespace BrainthreadIDE 
{
	String ^ InterpreterProcess::GetCodeLocationArgument()
	{
		const int command_len_limit = 8000; 
		BrainthreadIDE::Language curLang = processWorkContext->settings->GetLanguage();

		//try resolve pragmas
		this->ResolvePragmas();

		//prepare source
		if(this->processWorkContext->fileContext 
		   && this->processWorkContext->fileContext->HasPhysicalFile() 
		   && GlobalOptions::Instance->SaveCodeBeforeRun == true
		   && runSelection == false)
		{ //source file
			//save code
			this->processWorkContext->fileContext->Content = this->Source;
			this->processWorkContext->fileContext->Save();
			
			if(GlobalOptions::Instance->CustomInterpreterFlag[curLang])
			{
				return String::Format("\"{0}\"", this->processWorkContext->fileContext->FilePath);
			}
			return String::Format("-s \"{0}\"", this->processWorkContext->fileContext->FilePath);
		}
		else if(this->Source->Length + this->processWorkContext->settings->GetAdditionalCommandsString()->Length > command_len_limit
			    || GlobalOptions::Instance->CustomInterpreterFlag[curLang] == true)
		{ // tmp source file
			FileContext ^ tmpRunFile = gcnew FileContext(FileContext::BaseDirectory() + "\\__run.b");

			//throw if error?
			tmpRunFile->Content = this->Source;
			tmpRunFile->Save();

			if(GlobalOptions::Instance->CustomInterpreterFlag[curLang])
			{
				return String::Format("\"{0}\"", tmpRunFile->FilePath);
			}
			return String::Format("-s \"{0}\"", tmpRunFile->FilePath);
		}
		else //raw source
		{
			return String::Format("-i \"{0}\"", this->Source);
		}
	}

	void InterpreterProcess::ResolvePragmas()
	{
		PragmaResolver ^ pragmaResolver = gcnew PragmaResolver(this->Source, GlobalOptions::Instance->Plugins);

		if(pragmaResolver->HasPragmas() /*&& this->runSelection == false*/) {
				pragmaResolver->Resolve(processWorkContext, this->processWorkContext->settings->GetLanguage());
		}
	}

	void InterpreterProcess::MoveProcessWindow()
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

		if(this->process && false == this->process->HasExited)
			::SetWindowPos((HWND)this->process->MainWindowHandle.ToPointer(), hia, x, y, 0, 0, flags); 
	}

}