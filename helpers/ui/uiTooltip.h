#pragma once

namespace BrainthreadIDE 
{
	using namespace System;
	
	public ref class uiTooltip 
	{
	public: static void Set(System::Windows::Forms::Control ^ c, String ^ tt_text)
			{
				System::Windows::Forms::ToolTip^ toolTip = gcnew System::Windows::Forms::ToolTip;
				toolTip->AutoPopDelay = autoPopDelay;
				toolTip->InitialDelay = initialDelay;
				toolTip->ReshowDelay = reshowDelay;
				  
				toolTip->ShowAlways = true;

				toolTip->SetToolTip(c, tt_text );
			}
	private: 
		    static int autoPopDelay = 5000;
			static int initialDelay = 450;
			static int reshowDelay = 750;
	};
}