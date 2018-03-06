#pragma once

#include "SyntaxHighLighter.h"
#include "WorkContexts.h"
#include "options/GlobalOptions.h"

namespace BrainthreadIDE {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormOptions
	/// </summary>
	public ref class FormOptions : public System::Windows::Forms::Form
	{
	public:
		FormOptions(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			optionsAreSaved = false;
			editorTextBoxTest->richTextBox->ReadOnly = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormOptions()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControlOptions;
	protected: 
	private: System::Windows::Forms::TabPage^  tabPageVisualOptions;
	private: System::Windows::Forms::TabPage^  tabPageInterpreterOptions;
	private: System::Windows::Forms::GroupBox^  groupBoxInterpreter;

	private: System::Windows::Forms::Label^  labelUnlockInfo;


	private: System::Windows::Forms::Button^  buttonSaveOptions;
	private: System::Windows::Forms::Button^  buttonCancelOptions;
	private: System::Windows::Forms::GroupBox^  groupBoxVisual;
	private: System::Windows::Forms::Button^  buttonSetFont;
	private: System::Windows::Forms::Button^  buttonSetGutterBackColor;
	private: System::Windows::Forms::Button^  buttonSetGutterForeColor;
	private: System::Windows::Forms::Button^  buttonSetBackColor;


	private: EditorTextBox::EditorTextBoxControl^  editorTextBoxTest;
	private: System::Windows::Forms::Label^  labelSyntaxStyle;
	private: System::Windows::Forms::ComboBox^  comboBoxSyntaxHighlightStyle;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		bool optionsAreSaved;

		literal String ^ cLabelSyntaxStyle = "Syntax highlighting style:";
		literal String ^ cLogOpenDialogFilters = "Log files|*.log;*.txt|All files|*.*";
		literal String ^ cIntOpenDialogFilters = "Applications (*.exe)|*.exe";

	private: System::Windows::Forms::GroupBox^  groupBoxLog;
	private: System::Windows::Forms::Button^  buttonGetLogPath;
	private: System::Windows::Forms::CheckBox^  checkBoxReadLog;


	private: System::Windows::Forms::TabPage^  tabPageDebuggerOptions;
	private: System::Windows::Forms::GroupBox^  groupBoxDebugger;
	private: System::Windows::Forms::CheckBox^  checkBoxTraceEachTread;
	private: System::Windows::Forms::TabPage^  tabPagePliginsOptions;
	private: System::Windows::Forms::CheckBox^  checkBoxThrdMemOnClick;
	private: System::Windows::Forms::GroupBox^  groupBoxDebuggerWndPos;


	private: System::Windows::Forms::ComboBox^  comboBoxDebugeeWindowPos;
	private: System::Windows::Forms::Label^  labelDebugeePos;
	private: System::Windows::Forms::GroupBox^  groupBoxInops;
	private: System::Windows::Forms::CheckBox^  checkBoxSaveBeforeRun;
	private: System::Windows::Forms::GroupBox^  groupBoxPlugins;
	private: System::Windows::Forms::SplitContainer^  splitContainerPlugins;
	private: System::Windows::Forms::CheckedListBox^  checkedListBoxPlugins;
	private: System::Windows::Forms::TextBox^  textBoxPluginDesc;

	private: System::Windows::Forms::CheckBox^  checkBoxPauseAfterRun;
	private: System::Windows::Forms::Label^  labelPluginInfo;
	private: System::Windows::Forms::Button^  buttonCheckAllPlugins;
	private: System::Windows::Forms::Label^  labelDebugeeStyle;
	private: System::Windows::Forms::ComboBox^  comboBoxDebugeeWindowStyle;
	private: System::Windows::Forms::Button^  buttonDefault;
	private: System::Windows::Forms::DataGridView^  dataGridViewInterpreter;



	private: System::Windows::Forms::CheckBox^  checkBoxNotPurgeOutputLog;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxLogPath;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewButtonColumn^  Column4;
	private: System::Windows::Forms::Button^  buttonSortPluginList;
	private: System::Windows::Forms::CheckBox^  checkBoxApplyTheme;
	private: System::Windows::Forms::Label^  labelThreadName;
	private: System::Windows::Forms::CheckBox^  checkBoxDebugSelection;
	private: System::Windows::Forms::ComboBox^  comboBoxThreadNames;





















	literal String ^ cEditorExampleCode = "+(++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.)"
		"\nThis function of ID \"1\" prints Hello World\n>[-]{ Spawn some threads\n"
		"[-<]+* call function 1\n &>+& ^ :"											;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tabControlOptions = (gcnew System::Windows::Forms::TabControl());
			this->tabPageVisualOptions = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxVisual = (gcnew System::Windows::Forms::GroupBox());
			this->checkBoxApplyTheme = (gcnew System::Windows::Forms::CheckBox());
			this->labelSyntaxStyle = (gcnew System::Windows::Forms::Label());
			this->comboBoxSyntaxHighlightStyle = (gcnew System::Windows::Forms::ComboBox());
			this->buttonSetFont = (gcnew System::Windows::Forms::Button());
			this->buttonSetGutterBackColor = (gcnew System::Windows::Forms::Button());
			this->buttonSetGutterForeColor = (gcnew System::Windows::Forms::Button());
			this->buttonSetBackColor = (gcnew System::Windows::Forms::Button());
			this->editorTextBoxTest = (gcnew EditorTextBox::EditorTextBoxControl());
			this->tabPageInterpreterOptions = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxInops = (gcnew System::Windows::Forms::GroupBox());
			this->checkBoxNotPurgeOutputLog = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxPauseAfterRun = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxSaveBeforeRun = (gcnew System::Windows::Forms::CheckBox());
			this->groupBoxLog = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxLogPath = (gcnew System::Windows::Forms::TextBox());
			this->buttonGetLogPath = (gcnew System::Windows::Forms::Button());
			this->checkBoxReadLog = (gcnew System::Windows::Forms::CheckBox());
			this->groupBoxInterpreter = (gcnew System::Windows::Forms::GroupBox());
			this->dataGridViewInterpreter = (gcnew System::Windows::Forms::DataGridView());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
			this->labelUnlockInfo = (gcnew System::Windows::Forms::Label());
			this->tabPageDebuggerOptions = (gcnew System::Windows::Forms::TabPage());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBoxDebuggerWndPos = (gcnew System::Windows::Forms::GroupBox());
			this->labelDebugeeStyle = (gcnew System::Windows::Forms::Label());
			this->comboBoxDebugeeWindowStyle = (gcnew System::Windows::Forms::ComboBox());
			this->labelDebugeePos = (gcnew System::Windows::Forms::Label());
			this->comboBoxDebugeeWindowPos = (gcnew System::Windows::Forms::ComboBox());
			this->groupBoxDebugger = (gcnew System::Windows::Forms::GroupBox());
			this->labelThreadName = (gcnew System::Windows::Forms::Label());
			this->checkBoxDebugSelection = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxThrdMemOnClick = (gcnew System::Windows::Forms::CheckBox());
			this->comboBoxThreadNames = (gcnew System::Windows::Forms::ComboBox());
			this->checkBoxTraceEachTread = (gcnew System::Windows::Forms::CheckBox());
			this->tabPagePliginsOptions = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxPlugins = (gcnew System::Windows::Forms::GroupBox());
			this->buttonSortPluginList = (gcnew System::Windows::Forms::Button());
			this->labelPluginInfo = (gcnew System::Windows::Forms::Label());
			this->buttonCheckAllPlugins = (gcnew System::Windows::Forms::Button());
			this->splitContainerPlugins = (gcnew System::Windows::Forms::SplitContainer());
			this->checkedListBoxPlugins = (gcnew System::Windows::Forms::CheckedListBox());
			this->textBoxPluginDesc = (gcnew System::Windows::Forms::TextBox());
			this->buttonSaveOptions = (gcnew System::Windows::Forms::Button());
			this->buttonCancelOptions = (gcnew System::Windows::Forms::Button());
			this->buttonDefault = (gcnew System::Windows::Forms::Button());
			this->tabControlOptions->SuspendLayout();
			this->tabPageVisualOptions->SuspendLayout();
			this->groupBoxVisual->SuspendLayout();
			this->tabPageInterpreterOptions->SuspendLayout();
			this->groupBoxInops->SuspendLayout();
			this->groupBoxLog->SuspendLayout();
			this->groupBoxInterpreter->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewInterpreter))->BeginInit();
			this->tabPageDebuggerOptions->SuspendLayout();
			this->groupBoxDebuggerWndPos->SuspendLayout();
			this->groupBoxDebugger->SuspendLayout();
			this->tabPagePliginsOptions->SuspendLayout();
			this->groupBoxPlugins->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerPlugins))->BeginInit();
			this->splitContainerPlugins->Panel1->SuspendLayout();
			this->splitContainerPlugins->Panel2->SuspendLayout();
			this->splitContainerPlugins->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControlOptions
			// 
			this->tabControlOptions->Controls->Add(this->tabPageVisualOptions);
			this->tabControlOptions->Controls->Add(this->tabPageInterpreterOptions);
			this->tabControlOptions->Controls->Add(this->tabPageDebuggerOptions);
			this->tabControlOptions->Controls->Add(this->tabPagePliginsOptions);
			this->tabControlOptions->Location = System::Drawing::Point(12, 12);
			this->tabControlOptions->Name = L"tabControlOptions";
			this->tabControlOptions->SelectedIndex = 0;
			this->tabControlOptions->Size = System::Drawing::Size(727, 431);
			this->tabControlOptions->TabIndex = 0;
			// 
			// tabPageVisualOptions
			// 
			this->tabPageVisualOptions->Controls->Add(this->groupBoxVisual);
			this->tabPageVisualOptions->Location = System::Drawing::Point(4, 22);
			this->tabPageVisualOptions->Name = L"tabPageVisualOptions";
			this->tabPageVisualOptions->Padding = System::Windows::Forms::Padding(3);
			this->tabPageVisualOptions->Size = System::Drawing::Size(719, 405);
			this->tabPageVisualOptions->TabIndex = 0;
			this->tabPageVisualOptions->Text = L"Visual";
			this->tabPageVisualOptions->UseVisualStyleBackColor = true;
			// 
			// groupBoxVisual
			// 
			this->groupBoxVisual->Controls->Add(this->checkBoxApplyTheme);
			this->groupBoxVisual->Controls->Add(this->labelSyntaxStyle);
			this->groupBoxVisual->Controls->Add(this->comboBoxSyntaxHighlightStyle);
			this->groupBoxVisual->Controls->Add(this->buttonSetFont);
			this->groupBoxVisual->Controls->Add(this->buttonSetGutterBackColor);
			this->groupBoxVisual->Controls->Add(this->buttonSetGutterForeColor);
			this->groupBoxVisual->Controls->Add(this->buttonSetBackColor);
			this->groupBoxVisual->Controls->Add(this->editorTextBoxTest);
			this->groupBoxVisual->Location = System::Drawing::Point(20, 21);
			this->groupBoxVisual->Name = L"groupBoxVisual";
			this->groupBoxVisual->Size = System::Drawing::Size(676, 362);
			this->groupBoxVisual->TabIndex = 0;
			this->groupBoxVisual->TabStop = false;
			this->groupBoxVisual->Text = L"Visual";
			// 
			// checkBoxApplyTheme
			// 
			this->checkBoxApplyTheme->AutoSize = true;
			this->checkBoxApplyTheme->Checked = true;
			this->checkBoxApplyTheme->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxApplyTheme->Location = System::Drawing::Point(25, 89);
			this->checkBoxApplyTheme->Name = L"checkBoxApplyTheme";
			this->checkBoxApplyTheme->Size = System::Drawing::Size(119, 17);
			this->checkBoxApplyTheme->TabIndex = 8;
			this->checkBoxApplyTheme->Text = L"Apply color theme";
			this->checkBoxApplyTheme->UseVisualStyleBackColor = true;
			// 
			// labelSyntaxStyle
			// 
			this->labelSyntaxStyle->AutoSize = true;
			this->labelSyntaxStyle->Location = System::Drawing::Point(22, 34);
			this->labelSyntaxStyle->Name = L"labelSyntaxStyle";
			this->labelSyntaxStyle->Size = System::Drawing::Size(169, 13);
			this->labelSyntaxStyle->TabIndex = 7;
			this->labelSyntaxStyle->Text = L"Select syntax highlighting style:";
			// 
			// comboBoxSyntaxHighlightStyle
			// 
			this->comboBoxSyntaxHighlightStyle->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxSyntaxHighlightStyle->FormattingEnabled = true;
			this->comboBoxSyntaxHighlightStyle->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"Default style", L"Light style", 
				L"Dark style", L"Simple synteax", L"No syntax highlighting"});
			this->comboBoxSyntaxHighlightStyle->Location = System::Drawing::Point(25, 61);
			this->comboBoxSyntaxHighlightStyle->Name = L"comboBoxSyntaxHighlightStyle";
			this->comboBoxSyntaxHighlightStyle->Size = System::Drawing::Size(166, 21);
			this->comboBoxSyntaxHighlightStyle->TabIndex = 6;
			this->comboBoxSyntaxHighlightStyle->SelectedIndexChanged += gcnew System::EventHandler(this, &FormOptions::comboBoxSyntaxHighlightStyle_SelectedIndexChanged);
			// 
			// buttonSetFont
			// 
			this->buttonSetFont->Location = System::Drawing::Point(25, 128);
			this->buttonSetFont->Name = L"buttonSetFont";
			this->buttonSetFont->Size = System::Drawing::Size(166, 23);
			this->buttonSetFont->TabIndex = 5;
			this->buttonSetFont->Text = L"Font style";
			this->buttonSetFont->UseVisualStyleBackColor = true;
			this->buttonSetFont->Click += gcnew System::EventHandler(this, &FormOptions::buttonSetFont_Click);
			// 
			// buttonSetGutterBackColor
			// 
			this->buttonSetGutterBackColor->Location = System::Drawing::Point(25, 203);
			this->buttonSetGutterBackColor->Name = L"buttonSetGutterBackColor";
			this->buttonSetGutterBackColor->Size = System::Drawing::Size(166, 23);
			this->buttonSetGutterBackColor->TabIndex = 4;
			this->buttonSetGutterBackColor->Text = L"Margin color";
			this->buttonSetGutterBackColor->UseVisualStyleBackColor = true;
			this->buttonSetGutterBackColor->Click += gcnew System::EventHandler(this, &FormOptions::buttonSetForeColor_Click);
			// 
			// buttonSetGutterForeColor
			// 
			this->buttonSetGutterForeColor->Location = System::Drawing::Point(25, 232);
			this->buttonSetGutterForeColor->Name = L"buttonSetGutterForeColor";
			this->buttonSetGutterForeColor->Size = System::Drawing::Size(166, 23);
			this->buttonSetGutterForeColor->TabIndex = 3;
			this->buttonSetGutterForeColor->Text = L"Margin font color";
			this->buttonSetGutterForeColor->UseVisualStyleBackColor = true;
			this->buttonSetGutterForeColor->Click += gcnew System::EventHandler(this, &FormOptions::buttonSetForeColor_Click);
			// 
			// buttonSetBackColor
			// 
			this->buttonSetBackColor->Location = System::Drawing::Point(25, 157);
			this->buttonSetBackColor->Name = L"buttonSetBackColor";
			this->buttonSetBackColor->Size = System::Drawing::Size(166, 23);
			this->buttonSetBackColor->TabIndex = 2;
			this->buttonSetBackColor->Text = L"Background Color";
			this->buttonSetBackColor->UseVisualStyleBackColor = true;
			this->buttonSetBackColor->Click += gcnew System::EventHandler(this, &FormOptions::buttonSetForeColor_Click);
			// 
			// editorTextBoxTest
			// 
			this->editorTextBoxTest->BackColor = System::Drawing::SystemColors::ControlLight;
			this->editorTextBoxTest->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->editorTextBoxTest->GutterBackColor = System::Drawing::SystemColors::ControlLight;
			this->editorTextBoxTest->GutterForeColor = System::Drawing::Color::Black;
			this->editorTextBoxTest->GutterMargin = static_cast<System::UInt32>(6);
			this->editorTextBoxTest->Location = System::Drawing::Point(213, 34);
			this->editorTextBoxTest->Name = L"editorTextBoxTest";
			this->editorTextBoxTest->Size = System::Drawing::Size(441, 310);
			this->editorTextBoxTest->TabIndent = false;
			this->editorTextBoxTest->TabIndex = 0;
			this->editorTextBoxTest->UndoLimit = static_cast<System::UInt32>(0);
			// 
			// tabPageInterpreterOptions
			// 
			this->tabPageInterpreterOptions->Controls->Add(this->groupBoxInops);
			this->tabPageInterpreterOptions->Controls->Add(this->groupBoxLog);
			this->tabPageInterpreterOptions->Controls->Add(this->groupBoxInterpreter);
			this->tabPageInterpreterOptions->Location = System::Drawing::Point(4, 22);
			this->tabPageInterpreterOptions->Name = L"tabPageInterpreterOptions";
			this->tabPageInterpreterOptions->Padding = System::Windows::Forms::Padding(3);
			this->tabPageInterpreterOptions->Size = System::Drawing::Size(719, 405);
			this->tabPageInterpreterOptions->TabIndex = 1;
			this->tabPageInterpreterOptions->Text = L"Interpreter";
			this->tabPageInterpreterOptions->UseVisualStyleBackColor = true;
			// 
			// groupBoxInops
			// 
			this->groupBoxInops->Controls->Add(this->checkBoxNotPurgeOutputLog);
			this->groupBoxInops->Controls->Add(this->checkBoxPauseAfterRun);
			this->groupBoxInops->Controls->Add(this->checkBoxSaveBeforeRun);
			this->groupBoxInops->Location = System::Drawing::Point(20, 237);
			this->groupBoxInops->Name = L"groupBoxInops";
			this->groupBoxInops->Size = System::Drawing::Size(676, 54);
			this->groupBoxInops->TabIndex = 2;
			this->groupBoxInops->TabStop = false;
			this->groupBoxInops->Text = L"Options";
			// 
			// checkBoxNotPurgeOutputLog
			// 
			this->checkBoxNotPurgeOutputLog->AutoSize = true;
			this->checkBoxNotPurgeOutputLog->Location = System::Drawing::Point(516, 21);
			this->checkBoxNotPurgeOutputLog->Name = L"checkBoxNotPurgeOutputLog";
			this->checkBoxNotPurgeOutputLog->Size = System::Drawing::Size(141, 17);
			this->checkBoxNotPurgeOutputLog->TabIndex = 2;
			this->checkBoxNotPurgeOutputLog->Text = L"Don\'t clear output log";
			this->checkBoxNotPurgeOutputLog->UseVisualStyleBackColor = true;
			this->checkBoxNotPurgeOutputLog->Visible = false;
			// 
			// checkBoxPauseAfterRun
			// 
			this->checkBoxPauseAfterRun->AutoSize = true;
			this->checkBoxPauseAfterRun->ForeColor = System::Drawing::Color::RoyalBlue;
			this->checkBoxPauseAfterRun->Location = System::Drawing::Point(22, 21);
			this->checkBoxPauseAfterRun->Name = L"checkBoxPauseAfterRun";
			this->checkBoxPauseAfterRun->Size = System::Drawing::Size(223, 17);
			this->checkBoxPauseAfterRun->TabIndex = 1;
			this->checkBoxPauseAfterRun->Text = L"Pause the program after the execution";
			this->checkBoxPauseAfterRun->UseVisualStyleBackColor = true;
			// 
			// checkBoxSaveBeforeRun
			// 
			this->checkBoxSaveBeforeRun->AutoSize = true;
			this->checkBoxSaveBeforeRun->Location = System::Drawing::Point(303, 21);
			this->checkBoxSaveBeforeRun->Name = L"checkBoxSaveBeforeRun";
			this->checkBoxSaveBeforeRun->Size = System::Drawing::Size(207, 17);
			this->checkBoxSaveBeforeRun->TabIndex = 0;
			this->checkBoxSaveBeforeRun->Text = L"Save the sourcefile before each run";
			this->checkBoxSaveBeforeRun->UseVisualStyleBackColor = true;
			// 
			// groupBoxLog
			// 
			this->groupBoxLog->Controls->Add(this->textBoxLogPath);
			this->groupBoxLog->Controls->Add(this->buttonGetLogPath);
			this->groupBoxLog->Controls->Add(this->checkBoxReadLog);
			this->groupBoxLog->Location = System::Drawing::Point(20, 297);
			this->groupBoxLog->Name = L"groupBoxLog";
			this->groupBoxLog->Size = System::Drawing::Size(676, 87);
			this->groupBoxLog->TabIndex = 1;
			this->groupBoxLog->TabStop = false;
			this->groupBoxLog->Text = L"Log file";
			// 
			// textBoxLogPath
			// 
			this->textBoxLogPath->Location = System::Drawing::Point(22, 44);
			this->textBoxLogPath->MaxLength = 2048;
			this->textBoxLogPath->Name = L"textBoxLogPath";
			this->textBoxLogPath->Size = System::Drawing::Size(565, 22);
			this->textBoxLogPath->TabIndex = 7;
			// 
			// buttonGetLogPath
			// 
			this->buttonGetLogPath->Location = System::Drawing::Point(610, 44);
			this->buttonGetLogPath->Name = L"buttonGetLogPath";
			this->buttonGetLogPath->Size = System::Drawing::Size(47, 23);
			this->buttonGetLogPath->TabIndex = 6;
			this->buttonGetLogPath->Text = L"...";
			this->buttonGetLogPath->UseVisualStyleBackColor = true;
			this->buttonGetLogPath->Click += gcnew System::EventHandler(this, &FormOptions::buttonGetLogPath_Click);
			// 
			// checkBoxReadLog
			// 
			this->checkBoxReadLog->AutoSize = true;
			this->checkBoxReadLog->Location = System::Drawing::Point(25, 21);
			this->checkBoxReadLog->Name = L"checkBoxReadLog";
			this->checkBoxReadLog->Size = System::Drawing::Size(147, 17);
			this->checkBoxReadLog->TabIndex = 5;
			this->checkBoxReadLog->Text = L"Read log after each run";
			this->checkBoxReadLog->UseVisualStyleBackColor = true;
			this->checkBoxReadLog->CheckedChanged += gcnew System::EventHandler(this, &FormOptions::checkBoxReadLog_CheckedChanged);
			// 
			// groupBoxInterpreter
			// 
			this->groupBoxInterpreter->Controls->Add(this->dataGridViewInterpreter);
			this->groupBoxInterpreter->Controls->Add(this->labelUnlockInfo);
			this->groupBoxInterpreter->Location = System::Drawing::Point(20, 21);
			this->groupBoxInterpreter->Name = L"groupBoxInterpreter";
			this->groupBoxInterpreter->Size = System::Drawing::Size(676, 210);
			this->groupBoxInterpreter->TabIndex = 0;
			this->groupBoxInterpreter->TabStop = false;
			this->groupBoxInterpreter->Text = L"Used interpreter";
			// 
			// dataGridViewInterpreter
			// 
			this->dataGridViewInterpreter->AllowUserToAddRows = false;
			this->dataGridViewInterpreter->AllowUserToDeleteRows = false;
			this->dataGridViewInterpreter->BackgroundColor = System::Drawing::SystemColors::Window;
			this->dataGridViewInterpreter->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewInterpreter->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->Column2, 
				this->Column3, this->Column4});
			this->dataGridViewInterpreter->EnableHeadersVisualStyles = false;
			this->dataGridViewInterpreter->Location = System::Drawing::Point(22, 26);
			this->dataGridViewInterpreter->MultiSelect = false;
			this->dataGridViewInterpreter->Name = L"dataGridViewInterpreter";
			this->dataGridViewInterpreter->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders;
			this->dataGridViewInterpreter->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->dataGridViewInterpreter->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
			this->dataGridViewInterpreter->ShowCellErrors = false;
			this->dataGridViewInterpreter->Size = System::Drawing::Size(635, 147);
			this->dataGridViewInterpreter->TabIndex = 3;
			this->dataGridViewInterpreter->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &FormOptions::dataGridViewInterpreter_CellContentClick);
			this->dataGridViewInterpreter->CellFormatting += gcnew System::Windows::Forms::DataGridViewCellFormattingEventHandler(this, &FormOptions::dataGridViewInterpreter_CellFormatting);
			// 
			// Column2
			// 
			this->Column2->Frozen = true;
			this->Column2->HeaderText = L"Interpreter path";
			this->Column2->MinimumWidth = 150;
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column2->Width = 375;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"Commands";
			this->Column3->MinimumWidth = 70;
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			this->Column3->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column3->Width = 70;
			// 
			// Column4
			// 
			this->Column4->HeaderText = L"change";
			this->Column4->MinimumWidth = 50;
			this->Column4->Name = L"Column4";
			this->Column4->ReadOnly = true;
			this->Column4->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->Column4->UseColumnTextForButtonValue = true;
			this->Column4->Width = 50;
			// 
			// labelUnlockInfo
			// 
			this->labelUnlockInfo->AutoSize = true;
			this->labelUnlockInfo->ForeColor = System::Drawing::Color::RoyalBlue;
			this->labelUnlockInfo->Location = System::Drawing::Point(19, 184);
			this->labelUnlockInfo->Name = L"labelUnlockInfo";
			this->labelUnlockInfo->Size = System::Drawing::Size(558, 13);
			this->labelUnlockInfo->TabIndex = 2;
			this->labelUnlockInfo->Text = L"Note that using another interpreter may cause losing the ability to debug and ana" 
				L"lyze your Brainfuck code.";
			// 
			// tabPageDebuggerOptions
			// 
			this->tabPageDebuggerOptions->Controls->Add(this->label1);
			this->tabPageDebuggerOptions->Controls->Add(this->groupBoxDebuggerWndPos);
			this->tabPageDebuggerOptions->Controls->Add(this->groupBoxDebugger);
			this->tabPageDebuggerOptions->Location = System::Drawing::Point(4, 22);
			this->tabPageDebuggerOptions->Name = L"tabPageDebuggerOptions";
			this->tabPageDebuggerOptions->Padding = System::Windows::Forms::Padding(3);
			this->tabPageDebuggerOptions->Size = System::Drawing::Size(719, 405);
			this->tabPageDebuggerOptions->TabIndex = 2;
			this->tabPageDebuggerOptions->Text = L"Debugger";
			this->tabPageDebuggerOptions->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::RoyalBlue;
			this->label1->Location = System::Drawing::Point(17, 289);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(330, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"You can change Threading options during debugging process.";
			// 
			// groupBoxDebuggerWndPos
			// 
			this->groupBoxDebuggerWndPos->Controls->Add(this->labelDebugeeStyle);
			this->groupBoxDebuggerWndPos->Controls->Add(this->comboBoxDebugeeWindowStyle);
			this->groupBoxDebuggerWndPos->Controls->Add(this->labelDebugeePos);
			this->groupBoxDebuggerWndPos->Controls->Add(this->comboBoxDebugeeWindowPos);
			this->groupBoxDebuggerWndPos->Location = System::Drawing::Point(20, 142);
			this->groupBoxDebuggerWndPos->Name = L"groupBoxDebuggerWndPos";
			this->groupBoxDebuggerWndPos->Size = System::Drawing::Size(676, 126);
			this->groupBoxDebuggerWndPos->TabIndex = 2;
			this->groupBoxDebuggerWndPos->TabStop = false;
			this->groupBoxDebuggerWndPos->Text = L"Debugee window apperance";
			// 
			// labelDebugeeStyle
			// 
			this->labelDebugeeStyle->AutoSize = true;
			this->labelDebugeeStyle->Location = System::Drawing::Point(22, 38);
			this->labelDebugeeStyle->Name = L"labelDebugeeStyle";
			this->labelDebugeeStyle->Size = System::Drawing::Size(80, 13);
			this->labelDebugeeStyle->TabIndex = 5;
			this->labelDebugeeStyle->Text = L"Window style:";
			// 
			// comboBoxDebugeeWindowStyle
			// 
			this->comboBoxDebugeeWindowStyle->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxDebugeeWindowStyle->FormattingEnabled = true;
			this->comboBoxDebugeeWindowStyle->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Default", L"Hide window", L"Always stay on top"});
			this->comboBoxDebugeeWindowStyle->Location = System::Drawing::Point(163, 35);
			this->comboBoxDebugeeWindowStyle->Name = L"comboBoxDebugeeWindowStyle";
			this->comboBoxDebugeeWindowStyle->Size = System::Drawing::Size(197, 21);
			this->comboBoxDebugeeWindowStyle->TabIndex = 4;
			// 
			// labelDebugeePos
			// 
			this->labelDebugeePos->AutoSize = true;
			this->labelDebugeePos->Location = System::Drawing::Point(22, 74);
			this->labelDebugeePos->Name = L"labelDebugeePos";
			this->labelDebugeePos->Size = System::Drawing::Size(100, 13);
			this->labelDebugeePos->TabIndex = 3;
			this->labelDebugeePos->Text = L"Window position:";
			// 
			// comboBoxDebugeeWindowPos
			// 
			this->comboBoxDebugeeWindowPos->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxDebugeeWindowPos->FormattingEnabled = true;
			this->comboBoxDebugeeWindowPos->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Default", L"Form center", L"Screen top left", 
				L"Align to right edge of form"});
			this->comboBoxDebugeeWindowPos->Location = System::Drawing::Point(163, 71);
			this->comboBoxDebugeeWindowPos->Name = L"comboBoxDebugeeWindowPos";
			this->comboBoxDebugeeWindowPos->Size = System::Drawing::Size(197, 21);
			this->comboBoxDebugeeWindowPos->TabIndex = 0;
			// 
			// groupBoxDebugger
			// 
			this->groupBoxDebugger->Controls->Add(this->labelThreadName);
			this->groupBoxDebugger->Controls->Add(this->checkBoxDebugSelection);
			this->groupBoxDebugger->Controls->Add(this->checkBoxThrdMemOnClick);
			this->groupBoxDebugger->Controls->Add(this->comboBoxThreadNames);
			this->groupBoxDebugger->Controls->Add(this->checkBoxTraceEachTread);
			this->groupBoxDebugger->Location = System::Drawing::Point(20, 21);
			this->groupBoxDebugger->Name = L"groupBoxDebugger";
			this->groupBoxDebugger->Size = System::Drawing::Size(676, 106);
			this->groupBoxDebugger->TabIndex = 0;
			this->groupBoxDebugger->TabStop = false;
			this->groupBoxDebugger->Text = L"Threading";
			// 
			// labelThreadName
			// 
			this->labelThreadName->AutoSize = true;
			this->labelThreadName->Location = System::Drawing::Point(365, 35);
			this->labelThreadName->Name = L"labelThreadName";
			this->labelThreadName->Size = System::Drawing::Size(81, 13);
			this->labelThreadName->TabIndex = 8;
			this->labelThreadName->Text = L"Thread names:";
			// 
			// checkBoxDebugSelection
			// 
			this->checkBoxDebugSelection->AutoSize = true;
			this->checkBoxDebugSelection->Location = System::Drawing::Point(368, 66);
			this->checkBoxDebugSelection->Name = L"checkBoxDebugSelection";
			this->checkBoxDebugSelection->Size = System::Drawing::Size(174, 17);
			this->checkBoxDebugSelection->TabIndex = 7;
			this->checkBoxDebugSelection->Text = L"Highlight current instruction";
			this->checkBoxDebugSelection->UseVisualStyleBackColor = true;
			// 
			// checkBoxThrdMemOnClick
			// 
			this->checkBoxThrdMemOnClick->AutoSize = true;
			this->checkBoxThrdMemOnClick->Location = System::Drawing::Point(22, 66);
			this->checkBoxThrdMemOnClick->Name = L"checkBoxThrdMemOnClick";
			this->checkBoxThrdMemOnClick->Size = System::Drawing::Size(210, 17);
			this->checkBoxThrdMemOnClick->TabIndex = 1;
			this->checkBoxThrdMemOnClick->Text = L"Click on thread tree to view memory";
			this->checkBoxThrdMemOnClick->UseVisualStyleBackColor = true;
			// 
			// comboBoxThreadNames
			// 
			this->comboBoxThreadNames->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxThreadNames->FormattingEnabled = true;
			this->comboBoxThreadNames->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"System thread ID", L"Zero-based order", 
				L"1-based order"});
			this->comboBoxThreadNames->Location = System::Drawing::Point(473, 32);
			this->comboBoxThreadNames->Name = L"comboBoxThreadNames";
			this->comboBoxThreadNames->Size = System::Drawing::Size(172, 21);
			this->comboBoxThreadNames->TabIndex = 6;
			// 
			// checkBoxTraceEachTread
			// 
			this->checkBoxTraceEachTread->AutoSize = true;
			this->checkBoxTraceEachTread->Location = System::Drawing::Point(22, 34);
			this->checkBoxTraceEachTread->Name = L"checkBoxTraceEachTread";
			this->checkBoxTraceEachTread->Size = System::Drawing::Size(237, 17);
			this->checkBoxTraceEachTread->TabIndex = 0;
			this->checkBoxTraceEachTread->Text = L"Automatically trace each spawned thread";
			this->checkBoxTraceEachTread->UseVisualStyleBackColor = true;
			// 
			// tabPagePliginsOptions
			// 
			this->tabPagePliginsOptions->Controls->Add(this->groupBoxPlugins);
			this->tabPagePliginsOptions->Location = System::Drawing::Point(4, 22);
			this->tabPagePliginsOptions->Name = L"tabPagePliginsOptions";
			this->tabPagePliginsOptions->Padding = System::Windows::Forms::Padding(3);
			this->tabPagePliginsOptions->Size = System::Drawing::Size(719, 405);
			this->tabPagePliginsOptions->TabIndex = 3;
			this->tabPagePliginsOptions->Text = L"Plugins";
			this->tabPagePliginsOptions->UseVisualStyleBackColor = true;
			// 
			// groupBoxPlugins
			// 
			this->groupBoxPlugins->Controls->Add(this->buttonSortPluginList);
			this->groupBoxPlugins->Controls->Add(this->labelPluginInfo);
			this->groupBoxPlugins->Controls->Add(this->buttonCheckAllPlugins);
			this->groupBoxPlugins->Controls->Add(this->splitContainerPlugins);
			this->groupBoxPlugins->Location = System::Drawing::Point(20, 21);
			this->groupBoxPlugins->Name = L"groupBoxPlugins";
			this->groupBoxPlugins->Size = System::Drawing::Size(676, 362);
			this->groupBoxPlugins->TabIndex = 1;
			this->groupBoxPlugins->TabStop = false;
			this->groupBoxPlugins->Text = L"Available plugins";
			// 
			// buttonSortPluginList
			// 
			this->buttonSortPluginList->Location = System::Drawing::Point(526, 32);
			this->buttonSortPluginList->Name = L"buttonSortPluginList";
			this->buttonSortPluginList->Size = System::Drawing::Size(127, 23);
			this->buttonSortPluginList->TabIndex = 4;
			this->buttonSortPluginList->Text = L"Sort plugin list";
			this->buttonSortPluginList->UseVisualStyleBackColor = true;
			this->buttonSortPluginList->Click += gcnew System::EventHandler(this, &FormOptions::buttonSortPluginList_Click);
			// 
			// labelPluginInfo
			// 
			this->labelPluginInfo->AutoSize = true;
			this->labelPluginInfo->Location = System::Drawing::Point(19, 37);
			this->labelPluginInfo->Name = L"labelPluginInfo";
			this->labelPluginInfo->Size = System::Drawing::Size(217, 13);
			this->labelPluginInfo->TabIndex = 3;
			this->labelPluginInfo->Text = L"Choosed plugins are allowed to execute.";
			// 
			// buttonCheckAllPlugins
			// 
			this->buttonCheckAllPlugins->Location = System::Drawing::Point(263, 32);
			this->buttonCheckAllPlugins->Name = L"buttonCheckAllPlugins";
			this->buttonCheckAllPlugins->Size = System::Drawing::Size(127, 23);
			this->buttonCheckAllPlugins->TabIndex = 2;
			this->buttonCheckAllPlugins->Text = L"Check all plugins";
			this->buttonCheckAllPlugins->UseVisualStyleBackColor = true;
			this->buttonCheckAllPlugins->Click += gcnew System::EventHandler(this, &FormOptions::checkBoxPluginAutoinstall_CheckedChanged);
			// 
			// splitContainerPlugins
			// 
			this->splitContainerPlugins->Location = System::Drawing::Point(22, 70);
			this->splitContainerPlugins->Name = L"splitContainerPlugins";
			// 
			// splitContainerPlugins.Panel1
			// 
			this->splitContainerPlugins->Panel1->Controls->Add(this->checkedListBoxPlugins);
			// 
			// splitContainerPlugins.Panel2
			// 
			this->splitContainerPlugins->Panel2->Controls->Add(this->textBoxPluginDesc);
			this->splitContainerPlugins->Size = System::Drawing::Size(631, 267);
			this->splitContainerPlugins->SplitterDistance = 233;
			this->splitContainerPlugins->TabIndex = 1;
			// 
			// checkedListBoxPlugins
			// 
			this->checkedListBoxPlugins->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->checkedListBoxPlugins->Dock = System::Windows::Forms::DockStyle::Fill;
			this->checkedListBoxPlugins->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->checkedListBoxPlugins->FormattingEnabled = true;
			this->checkedListBoxPlugins->Location = System::Drawing::Point(0, 0);
			this->checkedListBoxPlugins->Name = L"checkedListBoxPlugins";
			this->checkedListBoxPlugins->Size = System::Drawing::Size(233, 267);
			this->checkedListBoxPlugins->TabIndex = 0;
			this->checkedListBoxPlugins->Click += gcnew System::EventHandler(this, &FormOptions::checkedListBoxPlugins_Click);
			// 
			// textBoxPluginDesc
			// 
			this->textBoxPluginDesc->BackColor = System::Drawing::SystemColors::Info;
			this->textBoxPluginDesc->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBoxPluginDesc->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->textBoxPluginDesc->Location = System::Drawing::Point(0, 0);
			this->textBoxPluginDesc->Multiline = true;
			this->textBoxPluginDesc->Name = L"textBoxPluginDesc";
			this->textBoxPluginDesc->ReadOnly = true;
			this->textBoxPluginDesc->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxPluginDesc->Size = System::Drawing::Size(394, 267);
			this->textBoxPluginDesc->TabIndex = 0;
			this->textBoxPluginDesc->Text = L"Click on the list to show plugin\'s details";
			// 
			// buttonSaveOptions
			// 
			this->buttonSaveOptions->Location = System::Drawing::Point(522, 456);
			this->buttonSaveOptions->Name = L"buttonSaveOptions";
			this->buttonSaveOptions->Size = System::Drawing::Size(104, 23);
			this->buttonSaveOptions->TabIndex = 1;
			this->buttonSaveOptions->Text = L"Apply";
			this->buttonSaveOptions->UseVisualStyleBackColor = true;
			this->buttonSaveOptions->Click += gcnew System::EventHandler(this, &FormOptions::buttonSaveOptions_Click);
			// 
			// buttonCancelOptions
			// 
			this->buttonCancelOptions->Location = System::Drawing::Point(632, 456);
			this->buttonCancelOptions->Name = L"buttonCancelOptions";
			this->buttonCancelOptions->Size = System::Drawing::Size(104, 23);
			this->buttonCancelOptions->TabIndex = 2;
			this->buttonCancelOptions->Text = L"Cancel";
			this->buttonCancelOptions->UseVisualStyleBackColor = true;
			this->buttonCancelOptions->Click += gcnew System::EventHandler(this, &FormOptions::buttonCancelOptions_Click);
			// 
			// buttonDefault
			// 
			this->buttonDefault->Location = System::Drawing::Point(12, 456);
			this->buttonDefault->Name = L"buttonDefault";
			this->buttonDefault->Size = System::Drawing::Size(104, 23);
			this->buttonDefault->TabIndex = 3;
			this->buttonDefault->Text = L"Default";
			this->buttonDefault->UseVisualStyleBackColor = true;
			this->buttonDefault->Click += gcnew System::EventHandler(this, &FormOptions::buttonDefault_Click);
			// 
			// FormOptions
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(752, 501);
			this->Controls->Add(this->buttonDefault);
			this->Controls->Add(this->buttonCancelOptions);
			this->Controls->Add(this->buttonSaveOptions);
			this->Controls->Add(this->tabControlOptions);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FormOptions";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"IDE Options";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormOptions::FormOptions_FormClosing);
			this->Shown += gcnew System::EventHandler(this, &FormOptions::FormOptions_Shown);
			this->tabControlOptions->ResumeLayout(false);
			this->tabPageVisualOptions->ResumeLayout(false);
			this->groupBoxVisual->ResumeLayout(false);
			this->groupBoxVisual->PerformLayout();
			this->tabPageInterpreterOptions->ResumeLayout(false);
			this->groupBoxInops->ResumeLayout(false);
			this->groupBoxInops->PerformLayout();
			this->groupBoxLog->ResumeLayout(false);
			this->groupBoxLog->PerformLayout();
			this->groupBoxInterpreter->ResumeLayout(false);
			this->groupBoxInterpreter->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewInterpreter))->EndInit();
			this->tabPageDebuggerOptions->ResumeLayout(false);
			this->tabPageDebuggerOptions->PerformLayout();
			this->groupBoxDebuggerWndPos->ResumeLayout(false);
			this->groupBoxDebuggerWndPos->PerformLayout();
			this->groupBoxDebugger->ResumeLayout(false);
			this->groupBoxDebugger->PerformLayout();
			this->tabPagePliginsOptions->ResumeLayout(false);
			this->groupBoxPlugins->ResumeLayout(false);
			this->groupBoxPlugins->PerformLayout();
			this->splitContainerPlugins->Panel1->ResumeLayout(false);
			this->splitContainerPlugins->Panel2->ResumeLayout(false);
			this->splitContainerPlugins->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerPlugins))->EndInit();
			this->splitContainerPlugins->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

private: void refreshEditorColors(void) {

			DefaultSyntaxHighLighter ^ syntaxHighLighter;
			
			switch(comboBoxSyntaxHighlightStyle->SelectedIndex)
			{
				case 0: syntaxHighLighter = gcnew DefaultSyntaxColors(gcnew BrainthreadSyntaxHighLighter); break;
				case 1: syntaxHighLighter = gcnew LightSyntaxColors(gcnew BrainthreadSyntaxHighLighter); break;
				case 2: syntaxHighLighter = gcnew DarkSyntaxColors(gcnew BrainthreadSyntaxHighLighter); break;
				case 3: syntaxHighLighter = gcnew RandomSyntaxColors(gcnew BrainthreadSyntaxHighLighter, false); break;
				
				default: syntaxHighLighter = gcnew BrainthreadSyntaxHighLighter;
			}

			editorTextBoxTest->richTextBox->Text = cEditorExampleCode;
			syntaxHighLighter->HighLightAll(editorTextBoxTest->richTextBox);
		}

private: void visualiseGenuineInterpreters(void) {
			//DataGridViewCellStyle ^ myRowStyle = this->dataGridViewInterpreter->RowHeadersDefaultCellStyle;
			String ^ cell_value;
		
			for each(DataGridViewRow ^ row in this->dataGridViewInterpreter->Rows) 
			{
				row->Resizable = DataGridViewTriState::False;
				cell_value = dynamic_cast<String^>(row->Cells[0]->Value);

				if(String::IsNullOrEmpty(cell_value))
				{
					row->HeaderCell->Style->BackColor = Color::Wheat;
					row->Cells[1]->ReadOnly = false;
				}
				else if(false == GlobalOptions::Instance->IsGenuineInterpreterPath( cell_value ))
				{
					row->HeaderCell->Style->BackColor = Color::LightBlue;
					row->Cells[1]->ReadOnly = false;
				}
				else 
					row->Cells[1]->ReadOnly = true;
			}
		}

private: void buttonSetFont_Click(System::Object^  sender, System::EventArgs^  e) {

			FontDialog^ fontDialog = gcnew FontDialog;
			fontDialog->ShowColor = true;

			fontDialog->Font = editorTextBoxTest->richTextBox->Font;
			fontDialog->Color = editorTextBoxTest->richTextBox->ForeColor;

			if (fontDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				editorTextBoxTest->richTextBox->Font = fontDialog->Font;
				editorTextBoxTest->richTextBox->ForeColor = fontDialog->Color;
				this->refreshEditorColors();
			}
		}
private: void buttonSetForeColor_Click(System::Object^  sender, System::EventArgs^  e) {

			ColorDialog^ colorDialog = gcnew ColorDialog;
			colorDialog->AllowFullOpen = true;
		    colorDialog->ShowHelp = true;

			labelSyntaxStyle->Text = cLabelSyntaxStyle;
			//load current color to colordialog
			if(sender == buttonSetBackColor){
				colorDialog->Color = editorTextBoxTest->richTextBox->BackColor;
			}
			else if(sender == buttonSetGutterForeColor){
				colorDialog->Color = editorTextBoxTest->GutterForeColor;
			}
			else if(sender == buttonSetGutterBackColor){
				colorDialog->Color = editorTextBoxTest->GutterBackColor;
			}
			 
			//set new color
			if ( colorDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				if(sender == buttonSetBackColor){
					editorTextBoxTest->richTextBox->BackColor = colorDialog->Color;
				}
				else if(sender == buttonSetGutterForeColor){
					editorTextBoxTest->GutterForeColor = colorDialog->Color;
				}
				else if(sender == buttonSetGutterBackColor){
					editorTextBoxTest->GutterBackColor = colorDialog->Color;
				}

				this->refreshEditorColors();
			}
		}

private: void comboBoxSyntaxHighlightStyle_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 
			 if(checkBoxApplyTheme->Checked == true)
			 {
				switch(comboBoxSyntaxHighlightStyle->SelectedIndex)
				{
					case 2: //dark style
						editorTextBoxTest->richTextBox->ForeColor = System::Drawing::Color::White; 
						editorTextBoxTest->richTextBox->BackColor = System::Drawing::Color::Black; 
						editorTextBoxTest->GutterForeColor = System::Drawing::SystemColors::ControlLight;
						editorTextBoxTest->GutterBackColor = System::Drawing::Color::FromArgb( 0xff, 0x33, 0x33, 0x33 ); //System::Drawing::Color::Black;
						break;

					default: 
						editorTextBoxTest->richTextBox->ForeColor = System::Drawing::Color::Gray;  
						editorTextBoxTest->richTextBox->BackColor = System::Drawing::Color::White; 
						editorTextBoxTest->GutterForeColor = System::Drawing::Color::Black;
						editorTextBoxTest->GutterBackColor = System::Drawing::SystemColors::ControlLight;
						break;
				}
			 }
			 this->refreshEditorColors();
		 }

private: void FormOptions_Shown(System::Object^  sender, System::EventArgs^  e) {
			 
			 GlobalOptions::Instance->PairControls(gcnew array<System::Windows::Forms::Control ^>(14) {
																						editorTextBoxTest, 
																						comboBoxSyntaxHighlightStyle, 
																						dataGridViewInterpreter,
																						checkBoxSaveBeforeRun,
																						checkBoxPauseAfterRun,
																						textBoxLogPath,
																						checkBoxReadLog,
																						checkBoxTraceEachTread,
																						checkBoxThrdMemOnClick,
																						comboBoxThreadNames,
																						checkBoxDebugSelection,
																						comboBoxDebugeeWindowStyle,
																						comboBoxDebugeeWindowPos,
																						checkedListBoxPlugins
														
																						}
			 );
			 //load options to ocntrols
			 GlobalOptions::Instance->Load();
			 
			 //some styling
			 buttonSaveOptions->NotifyDefault(true);
			 this->visualiseGenuineInterpreters();
			 this->refreshEditorColors();

			 this->checkBoxReadLog_CheckedChanged(sender, e);
		 }

private: System::Void buttonCancelOptions_Click(System::Object^  sender, System::EventArgs^  e) {
			 //close form
			 this->Close();
		 }
private: void buttonSaveOptions_Click(System::Object^  sender, System::EventArgs^  e) {
			 //save and apply
			 if(String::IsNullOrEmpty(textBoxLogPath->Text) && checkBoxReadLog->Checked)
			 {
				 MessageBox::Show("You must choose a log file path", "The path is empty", MessageBoxButtons::OK, MessageBoxIcon::Information);
				 return;			 
			 }
			 
			 GlobalOptions::Instance->Apply();
			 optionsAreSaved = true;

			 this->Close();
		 }
private: System::Void buttonDefault_Click(System::Object^  sender, System::EventArgs^  e) {
			 //default settings

			 if(MessageBox::Show("You cannot rollback this action.\nContinue?", "Setting to defaults", MessageBoxButtons::YesNo, MessageBoxIcon::Question)
				 == System::Windows::Forms::DialogResult::Yes)
			 {
				GlobalOptions::Instance->Default();
				GlobalOptions::Instance->Load();			 
			 }
			 
			 this->visualiseGenuineInterpreters();
		 }
private: void FormOptions_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {

			 if(optionsAreSaved) {
				 WorkContextBroker::Instance->RefreshEditor();
			 }
		 }

private: void buttonGetLogPath_Click(System::Object^  sender, System::EventArgs^  e) {

			 OpenFileDialog ^ openFileDialog = gcnew OpenFileDialog();
			 openFileDialog->Filter = cLogOpenDialogFilters;
			 openFileDialog->Title = "Select a log file to read from";

			 if(String::IsNullOrEmpty(textBoxLogPath->Text) == false)
				 openFileDialog->InitialDirectory = FileContext::GetDirectory(textBoxLogPath->Text);
			 else
				 openFileDialog->RestoreDirectory = true;

			 if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK){
				textBoxLogPath->Text = openFileDialog->FileName;
			 }
		 }

private: System::Void checkedListBoxPlugins_Click(System::Object^  sender, System::EventArgs^  e) {
			  //show plugins description
			 if(checkedListBoxPlugins->Items->Count < 1){
				 textBoxPluginDesc->Text = "No plugin available. Place a .dll file with a plugin to /plugins folder then restart IDE.";
			 }
			 else
				 textBoxPluginDesc->Text = GlobalOptions::Instance->Plugins->PluginDescription( checkedListBoxPlugins->Text );
		 }
private: System::Void checkBoxPluginAutoinstall_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 //check all plugins
			 for(int i = 0; i < checkedListBoxPlugins->Items->Count; ++i) {
					checkedListBoxPlugins->SetItemCheckState(i, CheckState::Checked);    
			 }
		 }

private: System::Void dataGridViewInterpreter_CellFormatting(System::Object^  sender, System::Windows::Forms::DataGridViewCellFormattingEventArgs^  e) {
			 //toopltips 4 datagridview
			 DataGridViewCell ^ cell = dataGridViewInterpreter->Rows[e->RowIndex]->Cells[e->ColumnIndex];
			 if (e->Value && e->ColumnIndex < 2)
			 {
				 String ^ value = dynamic_cast<String^>(cell->Value);
				 if(String::IsNullOrEmpty(value)) 
					cell->ToolTipText = "no interpreter";
				 else
					cell->ToolTipText = value;
			 }
		 }
private: System::Void dataGridViewInterpreter_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
			 //datagridview button click
			 DataGridViewCell ^ path_cell = dataGridViewInterpreter->Rows[e->RowIndex]->Cells[0];

			 if (e->ColumnIndex == 2)
			 {
				  OpenFileDialog ^ openFileDialog = gcnew OpenFileDialog();
				  openFileDialog->Filter = cIntOpenDialogFilters;
			      openFileDialog->InitialDirectory = FileContext::GetDirectory(dynamic_cast<String^>(path_cell->Value));  
				  openFileDialog->Title = String::Format("Select your {0} interpreter app", dataGridViewInterpreter->Rows[e->RowIndex]->HeaderCell->Value);

				  if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK){
						path_cell->Value = openFileDialog->FileName;

						this->visualiseGenuineInterpreters();
				  }
			 }
		 }
private: System::Void checkBoxReadLog_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 //ui 
			 textBoxLogPath->Enabled = checkBoxReadLog->Checked;
			 buttonGetLogPath->Enabled = checkBoxReadLog->Checked;
		 }
private: System::Void buttonSortPluginList_Click(System::Object^  sender, System::EventArgs^  e) {
			 //sort lugin  list
			 checkedListBoxPlugins->Sorted = !checkedListBoxPlugins->Sorted;
		 }
};
}
