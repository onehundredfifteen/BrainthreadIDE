#pragma once

#include "file_io/FileContext.h"

namespace BrainthreadIDE {

	using namespace System;
	//using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	

	/// <summary>
	/// Summary for FormProtip
	/// </summary>
	public ref class FormProtip : public System::Windows::Forms::Form
	{
	public:
		FormProtip(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->random = gcnew Random();
			this->protip_index = 0;

			FileContext ^ protipFileContext = gcnew FileContext(FileContext::BaseDirectory() + "\\res\\protips.dat");

			if(protipFileContext->Open()) {
				this->protips = protipFileContext->Content->Split(gcnew array<Char>(1){'\n'}, StringSplitOptions::RemoveEmptyEntries);
				this->shuffleProtips();
			}
			else {
				this->protips = gcnew array<String ^>(1){"Find protips.dat file :)))"};
			}

			this->ActiveControl = buttonNextProtip;  
  
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormProtip()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox;
	protected: 
	private: System::Windows::Forms::Button^  buttonNextProtip;
	private: System::Windows::Forms::PictureBox^  pictureBox;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		Random^ random;
		array<String ^> ^ protips;
		int protip_index;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox = (gcnew System::Windows::Forms::TextBox());
			this->buttonNextProtip = (gcnew System::Windows::Forms::Button());
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// textBox
			// 
			this->textBox->BackColor = System::Drawing::SystemColors::Info;
			this->textBox->Cursor = System::Windows::Forms::Cursors::Hand;
			this->textBox->Font = (gcnew System::Drawing::Font(L"Lucida Console", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->textBox->Location = System::Drawing::Point(0, 85);
			this->textBox->Multiline = true;
			this->textBox->Name = L"textBox";
			this->textBox->ReadOnly = true;
			this->textBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox->Size = System::Drawing::Size(470, 192);
			this->textBox->TabIndex = 0;
			// 
			// buttonNextProtip
			// 
			this->buttonNextProtip->Location = System::Drawing::Point(243, 283);
			this->buttonNextProtip->Name = L"buttonNextProtip";
			this->buttonNextProtip->Size = System::Drawing::Size(215, 23);
			this->buttonNextProtip->TabIndex = 1;
			this->buttonNextProtip->Text = L"Nest protip";
			this->buttonNextProtip->UseVisualStyleBackColor = true;
			this->buttonNextProtip->Click += gcnew System::EventHandler(this, &FormProtip::buttonNextProtip_Click);
			// 
			// pictureBox
			// 
			this->pictureBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->pictureBox->Location = System::Drawing::Point(0, 0);
			this->pictureBox->Name = L"pictureBox";
			this->pictureBox->Size = System::Drawing::Size(470, 79);
			this->pictureBox->TabIndex = 2;
			this->pictureBox->TabStop = false;
			// 
			// FormProtip
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(470, 317);
			this->Controls->Add(this->pictureBox);
			this->Controls->Add(this->buttonNextProtip);
			this->Controls->Add(this->textBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FormProtip";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Protip";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:  void getNextProtip() {

				 this->textBox->Text = this->protips[ protip_index++ ];

				 if(protip_index >= this->protips->Length)
					 protip_index = 0;
			 }
	public:  void behaveAsLangInfoForm() {

				 FileContext ^ protipFileContext = gcnew FileContext(FileContext::BaseDirectory() + "\\res\\langdesc.dat");
				 if(protipFileContext->Open()) {
					this->textBox->Text = protipFileContext->Content;
				 }

				 this->textBox->Text += "\r\n\r\n\tlink to esoalng";
				 this->Text = L"About Brainthread language";
			 }
	private: void shuffleProtips() {

				 for (int i = 0; i < this->protips->Length; ++i)
				 {
					 int n = this->random->Next(0, this->protips->Length);
					 String ^ tmp_str = this->protips[n];
					 this->protips[n] = this->protips[i];
					 this->protips[i] = tmp_str;
				 }
			 }
	private: System::Void buttonNextProtip_Click(System::Object^  sender, System::EventArgs^  e) {

				 getNextProtip();
			 }
	};
}
