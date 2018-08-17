#pragma once

#include "uiMenuButtonGenerator.h"

namespace BrainthreadIDE 
{
	using namespace System;

	public ref class uiMenuRadioButtonGenerator : uiMenuButtonGenerator
	{
	private:
		System::Windows::Forms::ToolStripMenuItem ^ root;

	public:
		uiMenuRadioButtonGenerator(System::Windows::Forms::ToolStripMenuItem^ menuItem, array<String ^> ^ btn_captions, EventHandler ^ btn_event, int index_to_check)
		{
			this->root = menuItem;

			Generate(root, btn_captions, btn_event);
			
			//attach events
			for each(ToolStripMenuItem ^ tsMenuItem in root->DropDownItems)
			{
				tsMenuItem->Click += gcnew System::EventHandler(this, &uiMenuRadioButtonGenerator::radioMenuItem_Click); //have to invert events queue
			}	

			this->ResetSelection(index_to_check);
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

	public: void ResetSelection(int index) {
			 
			 int i = 0;

			 for each(ToolStripMenuItem ^ tsMenuItem in this->root->DropDownItems)
			 {
				 tsMenuItem->Checked = (i++ == index);
			 }
		 }
	};
}