#pragma once

namespace BrainthreadIDE 
{
	using namespace System;

	public ref class uiMenuButtonGenerator
	{
	public:
		static void Generate(System::Windows::Forms::ToolStripMenuItem^ menuItem, array<String ^> ^ btn_captions, EventHandler ^ btn_event)
		{
			if(nullptr == btn_captions || btn_captions->Length == 0)
			{
				ToolStripMenuItem ^ empty_item = gcnew ToolStripMenuItem("(none)");
				empty_item->Enabled = false;

				menuItem->DropDownItems->Add(empty_item);
				return;
			}
			
			for each(String ^ caption in btn_captions)
			{
				menuItem->DropDownItems->Add(gcnew ToolStripMenuItem(caption, 
											 nullptr,  
											 btn_event));
			}	
		}

		static void Generate(System::Windows::Forms::ToolStripMenuItem^ menuItem, array<String ^> ^ btn_captions, EventHandler ^ btn_event, bool clear)
		{
			if(clear)
			{
				menuItem->DropDownItems->Clear();
			}
			
			Generate(menuItem, btn_captions, btn_event);
		}
	};

}