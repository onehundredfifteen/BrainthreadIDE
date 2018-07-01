#pragma once

#include "../WorkContexts.h"

namespace BrainthreadIDE 
{
	using namespace System;

	public ref class BasicProcess abstract
	{
	public:
		BasicProcess()
		{
			this->worker = gcnew System::ComponentModel::BackgroundWorker();

			this->worker->WorkerReportsProgress = false;
			this->processWorkContext = WorkContextBroker::Instance->GetCurrentContext();
			
			this->AttachWorkerEvents();
		}

		virtual bool Launch() = 0;

		virtual void Stop() {
			return;	
		}

	protected:
		virtual void AttachWorkerEvents() {
			return;	
		}

	public:
		event System::EventHandler ^ OnStart;
		event System::EventHandler ^ OnComplete;

	protected:
		WorkContext ^ processWorkContext;
		System::ComponentModel::BackgroundWorker ^ worker;

	public:
		property WorkContext ^ ProcessWorkContext
		{
			WorkContext ^ get()
			{
				return processWorkContext;
			}
		}

		property bool Working
		{
			bool get()
			{
				return (this->worker && this->worker->IsBusy);
			}
		}
	};
}
