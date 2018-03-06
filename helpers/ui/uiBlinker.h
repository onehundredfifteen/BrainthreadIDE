#pragma once

namespace BrainthreadIDE
{
    using namespace System;	
	using namespace System::Windows::Forms;

	public ref class uiBlinker
	{
	public:	
		uiBlinker(DataGridView ^ _control,  System::Drawing::Color _color) : control(_control), blink_color(_color)
		{
			sleeper = gcnew System::ComponentModel::BackgroundWorker();

			default_color = control->DefaultCellStyle->SelectionBackColor;
			color = blink_color;

			sleeper->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &uiBlinker::sleeper_DoWork);
			sleeper->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &uiBlinker::sleeper_RunWorkerCompleted);
		}

	protected:
		DataGridView ^ control;
		System::ComponentModel::BackgroundWorker ^ sleeper;
		System::Drawing::Color color, default_color, blink_color;
		int interval;
	
	public: void Blink(int _interval) {
				 
			control->DefaultCellStyle->SelectionBackColor = color;
			interval = _interval;
			sleeper->RunWorkerAsync();
		}

	private: void sleeper_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
		{
			Thread::Sleep(interval); 
		}

	private: void sleeper_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) 
		{
			control->DefaultCellStyle->SelectionBackColor = default_color;
		}

	};

	
}