#pragma once

#include "WorkContexts.h"
#include "helpers/BtConstantsCreator.h"
#include "helpers/ui/uiBevel.h"


namespace BrainthreadIDE {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormInsert
	/// </summary>
	public ref class FormInsert : public System::Windows::Forms::Form
	{
	public:
		FormInsert(void)
		{
			InitializeComponent();
			
			uiHorizontalLineBevel ^ bevel = gcnew uiHorizontalLineBevel(groupBox, 17, 78);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormInsert()
		{
			if (components)
			{
				delete components;
			}
		}


	private: System::Windows::Forms::Button^  buttonCancel;
	private: System::Windows::Forms::Button^  buttonInsert;
	private: System::Windows::Forms::GroupBox^  groupBox;
	private: TextBoxPlaceholder::TextBoxPlaceholderControl^  textBoxValue;
	private: System::Windows::Forms::CheckBox^  checkBoxOpCloseAfterInsert;
	private: System::Windows::Forms::CheckBox^  checkBoxOpOver;
	private: System::Windows::Forms::CheckBox^  checkBoxOpNewline;
	private: System::Windows::Forms::CheckBox^  checkBoxOpNull;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->buttonCancel = (gcnew System::Windows::Forms::Button());
			this->buttonInsert = (gcnew System::Windows::Forms::Button());
			this->groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxValue = (gcnew TextBoxPlaceholder::TextBoxPlaceholderControl());
			this->checkBoxOpCloseAfterInsert = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxOpOver = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxOpNewline = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxOpNull = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// buttonCancel
			// 
			this->buttonCancel->Location = System::Drawing::Point(230, 147);
			this->buttonCancel->Name = L"buttonCancel";
			this->buttonCancel->Size = System::Drawing::Size(75, 23);
			this->buttonCancel->TabIndex = 3;
			this->buttonCancel->Text = L"Cancel";
			this->buttonCancel->UseVisualStyleBackColor = true;
			this->buttonCancel->Click += gcnew System::EventHandler(this, &FormInsert::buttonCancel_Click);
			// 
			// buttonInsert
			// 
			this->buttonInsert->Location = System::Drawing::Point(149, 147);
			this->buttonInsert->Name = L"buttonInsert";
			this->buttonInsert->Size = System::Drawing::Size(75, 23);
			this->buttonInsert->TabIndex = 2;
			this->buttonInsert->Text = L"Insert";
			this->buttonInsert->UseVisualStyleBackColor = true;
			this->buttonInsert->Click += gcnew System::EventHandler(this, &FormInsert::buttonInsert_Click);
			// 
			// groupBox
			// 
			this->groupBox->Controls->Add(this->textBoxValue);
			this->groupBox->Controls->Add(this->checkBoxOpCloseAfterInsert);
			this->groupBox->Controls->Add(this->checkBoxOpOver);
			this->groupBox->Controls->Add(this->checkBoxOpNewline);
			this->groupBox->Controls->Add(this->checkBoxOpNull);
			this->groupBox->Location = System::Drawing::Point(19, 12);
			this->groupBox->Name = L"groupBox";
			this->groupBox->Size = System::Drawing::Size(286, 120);
			this->groupBox->TabIndex = 4;
			this->groupBox->TabStop = false;
			this->groupBox->Text = L"Type a number or a character";
			// 
			// textBoxValue
			// 
			this->textBoxValue->DefaultValueMode = false;
			this->textBoxValue->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->textBoxValue->Location = System::Drawing::Point(17, 19);
			this->textBoxValue->Name = L"textBoxValue";
			this->textBoxValue->NoWhitespaceMode = true;
			this->textBoxValue->PlaceHolderText = L"Type something to it convert into code";
			this->textBoxValue->PlaceHolderTextColor = System::Drawing::SystemColors::WindowText;
			this->textBoxValue->Size = System::Drawing::Size(252, 22);
			this->textBoxValue->TabIndex = 14;
			// 
			// checkBoxOpCloseAfterInsert
			// 
			this->checkBoxOpCloseAfterInsert->AutoSize = true;
			this->checkBoxOpCloseAfterInsert->Location = System::Drawing::Point(17, 89);
			this->checkBoxOpCloseAfterInsert->Name = L"checkBoxOpCloseAfterInsert";
			this->checkBoxOpCloseAfterInsert->Size = System::Drawing::Size(119, 17);
			this->checkBoxOpCloseAfterInsert->TabIndex = 13;
			this->checkBoxOpCloseAfterInsert->Text = L"Close once inserted";
			this->checkBoxOpCloseAfterInsert->UseVisualStyleBackColor = true;
			// 
			// checkBoxOpOver
			// 
			this->checkBoxOpOver->AutoSize = true;
			this->checkBoxOpOver->Enabled = false;
			this->checkBoxOpOver->Location = System::Drawing::Point(161, 55);
			this->checkBoxOpOver->Name = L"checkBoxOpOver";
			this->checkBoxOpOver->Size = System::Drawing::Size(100, 17);
			this->checkBoxOpOver->TabIndex = 11;
			this->checkBoxOpOver->Text = L"Allow underflow";
			this->checkBoxOpOver->UseVisualStyleBackColor = true;
			// 
			// checkBoxOpNewline
			// 
			this->checkBoxOpNewline->AutoSize = true;
			this->checkBoxOpNewline->Location = System::Drawing::Point(161, 89);
			this->checkBoxOpNewline->Name = L"checkBoxOpNewline";
			this->checkBoxOpNewline->Size = System::Drawing::Size(91, 17);
			this->checkBoxOpNewline->TabIndex = 10;
			this->checkBoxOpNewline->Text = L"Insert newline";
			this->checkBoxOpNewline->UseVisualStyleBackColor = true;
			// 
			// checkBoxOpNull
			// 
			this->checkBoxOpNull->AutoSize = true;
			this->checkBoxOpNull->Checked = true;
			this->checkBoxOpNull->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxOpNull->Location = System::Drawing::Point(17, 55);
			this->checkBoxOpNull->Name = L"checkBoxOpNull";
			this->checkBoxOpNull->Size = System::Drawing::Size(76, 17);
			this->checkBoxOpNull->TabIndex = 9;
			this->checkBoxOpNull->Text = L"Force zero";
			this->checkBoxOpNull->UseVisualStyleBackColor = true;
			// 
			// FormInsert
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(324, 182);
			this->Controls->Add(this->groupBox);
			this->Controls->Add(this->buttonCancel);
			this->Controls->Add(this->buttonInsert);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FormInsert";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Generate a constant";
			this->groupBox->ResumeLayout(false);
			this->groupBox->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

private: System::Void buttonInsert_Click(System::Object^  sender, System::EventArgs^  e) {

				 Int32 number;
				 String ^ to_insert;
				 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();

				 //generate
				 if (Int32::TryParse(textBoxValue->Text, number)) {
					 to_insert = BtConstantsCreator::GetNumberConstant(number)->ToString();
				 }
				 else if (textBoxValue->Text->Length == 1){
					 const auto asciiBytes = Encoding::ASCII->GetBytes(textBoxValue->Text->Substring(0,1));
					 to_insert = BtConstantsCreator::GetNumberConstant(asciiBytes[0])->ToString();
				 }
				 else if(textBoxValue->Text == "_list") { //insert list 15-1000

					 to_insert = "+([-]++++++++++.)>";
					 for(int i = 15; i <= 1000; ++i) {
						  to_insert += BtConstantsCreator::GetNumberConstant(i)->ToString() + ":>+*>\n ";
						  if(i == 255) {
							  to_insert += "\n";
						  }
					 }
				 }
				 else {
					 MessageBox::Show("Cannot insert strings - only numbers or individual characters are valid", "Type a number or a character", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					 return;
				 }

				 //force zero
				 if(checkBoxOpNull->Checked == true)
						 to_insert = "[-]" + to_insert;

				 //paste
				 if(currentWorkContext)
				 {			
					 //newline
					 if(checkBoxOpNewline->Checked == true)
						 to_insert = "\n" + to_insert;
					 
					 currentWorkContext->syntaxHighLighter->HighLightAndPaste(currentWorkContext->editorTextBox->richTextBox, 
																		      to_insert);
				 }

				 //close
				 if(checkBoxOpCloseAfterInsert->Checked == true)
				 {
					 Close();
				 }
			 }
private: System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e) {

			 Close();
		 }
};
}
