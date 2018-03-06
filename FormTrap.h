#pragma once

#include "debugger/Debugger.h"

namespace BrainthreadIDE {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormTrap
	/// </summary>
	public ref class FormTrap : public System::Windows::Forms::Form
	{
	public:
		FormTrap(int mem_index, String ^ pref_val)
		{
			InitializeComponent();
			
			this->textBoxMemoryCell->Text = mem_index.ToString();
			this->textBoxValue->Text = pref_val;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormTrap()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBox;
	protected: 
	private: System::Windows::Forms::RadioButton^  radioButton4;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::TextBox^  textBoxValue;
	private: System::Windows::Forms::TextBox^  textBoxMemoryCell;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  buttonRunToTrap;
	private: System::Windows::Forms::Button^  buttonCancel;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::CheckBox^  checkBoxAnyCell;
			 Int32 trap_value;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->textBoxValue = (gcnew System::Windows::Forms::TextBox());
			this->textBoxMemoryCell = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->buttonRunToTrap = (gcnew System::Windows::Forms::Button());
			this->buttonCancel = (gcnew System::Windows::Forms::Button());
			this->checkBoxAnyCell = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox
			// 
			this->groupBox->Controls->Add(this->checkBoxAnyCell);
			this->groupBox->Controls->Add(this->radioButton4);
			this->groupBox->Controls->Add(this->radioButton3);
			this->groupBox->Controls->Add(this->radioButton2);
			this->groupBox->Controls->Add(this->radioButton1);
			this->groupBox->Controls->Add(this->textBoxValue);
			this->groupBox->Controls->Add(this->textBoxMemoryCell);
			this->groupBox->Controls->Add(this->label1);
			this->groupBox->Location = System::Drawing::Point(12, 12);
			this->groupBox->Name = L"groupBox";
			this->groupBox->Size = System::Drawing::Size(282, 84);
			this->groupBox->TabIndex = 0;
			this->groupBox->TabStop = false;
			this->groupBox->Text = L"Define a memory trap";
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Location = System::Drawing::Point(172, 42);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(31, 17);
			this->radioButton4->TabIndex = 11;
			this->radioButton4->Text = L"<";
			this->radioButton4->UseVisualStyleBackColor = true;
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(135, 42);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(31, 17);
			this->radioButton3->TabIndex = 10;
			this->radioButton3->Text = L"≠";
			this->radioButton3->UseVisualStyleBackColor = true;
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(172, 19);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(31, 17);
			this->radioButton2->TabIndex = 9;
			this->radioButton2->Text = L">";
			this->radioButton2->UseVisualStyleBackColor = true;
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Checked = true;
			this->radioButton1->Location = System::Drawing::Point(135, 19);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(31, 17);
			this->radioButton1->TabIndex = 8;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"=";
			this->radioButton1->UseVisualStyleBackColor = true;
			// 
			// textBoxValue
			// 
			this->textBoxValue->Location = System::Drawing::Point(216, 28);
			this->textBoxValue->MaxLength = 5;
			this->textBoxValue->Name = L"textBoxValue";
			this->textBoxValue->Size = System::Drawing::Size(42, 20);
			this->textBoxValue->TabIndex = 7;
			this->textBoxValue->Text = L"0";
			this->textBoxValue->TextChanged += gcnew System::EventHandler(this, &FormTrap::textBoxValue_TextChanged);
			// 
			// textBoxMemoryCell
			// 
			this->textBoxMemoryCell->Location = System::Drawing::Point(76, 28);
			this->textBoxMemoryCell->MaxLength = 5;
			this->textBoxMemoryCell->Name = L"textBoxMemoryCell";
			this->textBoxMemoryCell->Size = System::Drawing::Size(42, 20);
			this->textBoxMemoryCell->TabIndex = 6;
			this->textBoxMemoryCell->Text = L"10000";
			this->textBoxMemoryCell->TabIndexChanged += gcnew System::EventHandler(this, &FormTrap::textBoxMemoryCell_TabIndexChanged);
			this->textBoxMemoryCell->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormTrap::textBoxMemoryCell_KeyPress);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(18, 31);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(52, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Cell index";
			// 
			// buttonRunToTrap
			// 
			this->buttonRunToTrap->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->buttonRunToTrap->Location = System::Drawing::Point(86, 102);
			this->buttonRunToTrap->Name = L"buttonRunToTrap";
			this->buttonRunToTrap->Size = System::Drawing::Size(127, 23);
			this->buttonRunToTrap->TabIndex = 1;
			this->buttonRunToTrap->Text = L"Set a trap and run";
			this->buttonRunToTrap->UseVisualStyleBackColor = true;
			// 
			// buttonCancel
			// 
			this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonCancel->Location = System::Drawing::Point(219, 102);
			this->buttonCancel->Name = L"buttonCancel";
			this->buttonCancel->Size = System::Drawing::Size(75, 23);
			this->buttonCancel->TabIndex = 2;
			this->buttonCancel->Text = L"Cancel";
			this->buttonCancel->UseVisualStyleBackColor = true;
			// 
			// checkBoxAnyCell
			// 
			this->checkBoxAnyCell->AutoSize = true;
			this->checkBoxAnyCell->Location = System::Drawing::Point(21, 54);
			this->checkBoxAnyCell->Name = L"checkBoxAnyCell";
			this->checkBoxAnyCell->Size = System::Drawing::Size(63, 17);
			this->checkBoxAnyCell->TabIndex = 12;
			this->checkBoxAnyCell->Text = L"Any cell";
			this->checkBoxAnyCell->UseVisualStyleBackColor = true;
			this->checkBoxAnyCell->CheckedChanged += gcnew System::EventHandler(this, &FormTrap::checkBoxAnyCell_CheckedChanged);
			// 
			// FormTrap
			// 
			this->AcceptButton = this->buttonRunToTrap;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->buttonCancel;
			this->ClientSize = System::Drawing::Size(306, 135);
			this->Controls->Add(this->buttonCancel);
			this->Controls->Add(this->buttonRunToTrap);
			this->Controls->Add(this->groupBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"FormTrap";
			this->Text = L"Memory trap";
			this->groupBox->ResumeLayout(false);
			this->groupBox->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

public: property int Value
		{
			int get() { 
				return trap_value;
			}
		}

public: property int Index
		{
			int get() {

				if(checkBoxAnyCell->Checked)
					return -1;

				return Convert::ToInt32(this->textBoxMemoryCell->Text);
			}
		}

public: property Debugger::CompareType Option
		{
			Debugger::CompareType get() { 

				if(this->radioButton2->Checked)
					return Debugger::CompareType::Bigger;
				else if(this->radioButton3->Checked)
					return Debugger::CompareType::NotEqual;
				else if(this->radioButton4->Checked)
					return Debugger::CompareType::Smaller;
				else
					return Debugger::CompareType::Equal;
			}
		}

private: System::Void textBoxMemoryCell_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {

			 if(!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
					e->Handled = true;
			 }
		 }

private: System::Void textBoxValue_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 //validate input

			 if (false == Int32::TryParse(textBoxValue->Text, this->trap_value)) {
				//conversion failed. char?	

				if(textBoxValue->Text->Length == 0){
					 textBoxValue->Text = "0";
					 this->trap_value = 0;

					 return;
				}
				else if(textBoxValue->Text->Length > 1) {
					textBoxValue->Text = textBoxValue->Text->Substring(0,1);
				}

				const auto asciiBytes = Encoding::ASCII->GetBytes(textBoxValue->Text->Substring(0,1));
				this->trap_value = asciiBytes[0];
			 }
		 }
private: System::Void textBoxMemoryCell_TabIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			 if(String::IsNullOrEmpty(textBoxMemoryCell->Text) || Convert::ToInt32(textBoxMemoryCell->Text) < 1)
			 {
				 textBoxMemoryCell->Text = "1";
			 }
		 }
private: System::Void checkBoxAnyCell_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

			 textBoxMemoryCell->Enabled = !checkBoxAnyCell->Checked;
		 }
};
}
