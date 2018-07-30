#include "StdAfx.h"
#include "IDEInitProcess.h"

#include "../helpers/BtOutputGenerator.h"
#include "../factories/FileContextFactory.h"

namespace BrainthreadIDE 
{
	bool IDEInitProcess::Launch()
	{
		if(this->Working)
			return false;

		this->OnStart(this, nullptr);
		
		AppArgumentsInit(); //cannot do this in thread
		worker->RunWorkerAsync();

		return true;
	}

	//task to do after IDE start
    void IDEInitProcess::worker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
		try
		{
			//BtOutputGenerator::BtOutputGenerator();
		}
		catch(...)
		{
			e->Result = false;
			return;
		}

		e->Result = true;
	}

	void IDEInitProcess::worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
	{
		this->OnComplete(this, nullptr);
	}

	void IDEInitProcess::AppArgumentsInit() 
	{
		//arg processing
		array<String^> ^ args = Environment::GetCommandLineArgs();

		if(args->Length == 2) //open file from arg
		{
			FileContext ^ argOpenFileContext = FileContextFactory::createFileContext(args[1]);
			if(argOpenFileContext->Open()) {
				WorkContextBroker::Instance->OpenPage(argOpenFileContext);
			}
		}
	}
}
