#pragma once

#include "uiMenuButtonGenerator.h"

namespace BrainthreadIDE 
{
	using namespace System;

	public ref class uiMenuRadioButtonGenerator : uiMenuButtonGenerator
	{
	public:
		uiMenuRadioButtonGenerator(System::Windows::Forms::ToolStripMenuItem^ menuItem, array<String ^> ^ btn_captions, EventHandler ^ btn_event, int index_to_check)
		{
			Generate(menuItem, btn_captions, btn_event);
			
			int i = 0;
			for each(ToolStripMenuItem ^ tsMenuItem in menuItem->DropDownItems)
			{
				tsMenuItem->Click +=  gcnew System::EventHandler(this, &uiMenuRadioButtonGenerator::radioMenuItem_Click); //have to invert events

				if(index_to_check == i++) {
					tsMenuItem->Checked = true;
				}
			}	
		}

	private: void radioMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 ToolStripMenuItem ^ sender_menu = cli::safe_cast<ToolStripMenuItem ^>(sender);
			 ToolStripMenuItem ^ parent_menu = cli::safe_cast<ToolStripMenuItem ^>(sender_menu->OwnerItem);

			 for each(ToolStripMenuItem ^ tsMenuItem in parent_menu->DropDownItems)
			 {
				 tsMenuItem->Checked = false;
			 }

			 sender_menu->Checked = true;
		 }
	};
}