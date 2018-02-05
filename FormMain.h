#pragma once

#include "FormOptions.h"
#include "FormInsert.h"
#include "FormProtip.h"

#include "launchers/RunCodeProcess.h"
#include "launchers/RunCodeSeparateProcess.h"
#include "launchers/CodeAnalysisProcess.h"
#include "launchers/CodeParseProcess.h"
#include "launchers/DebugCodeProcess.h"

#include "factories/SyntaxHighlighterFactory.h"
#include "factories/FileContextFactory.h"
#include "factories/RunProcessFactory.h"

#include "helpers/BtConstantsCreator.h"
#include "helpers/CodeContextConverter.h"
#include "helpers/FileSizeConverter.h"
#include "helpers/ui/uiMenuRadioButtonGenerator.h"
#include "helpers/ui/uiTooltip.h"
#include "helpers/ui/uiBevel.h"

#include "debugger/DebugTreeViewExpander.h"
#include "plugins/PragmaResolver.h"

#include "WorkContexts.h"




namespace BrainthreadIDE {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// App Main Form Class
	/// </summary>
	public ref class FormMain : public System::Windows::Forms::Form
	{
	public:
		FormMain(String ^ argFileToOpen)
		{
			InitializeComponent();
			
			//init workspace
			this->textBoxPhOptionsInput->PlaceHolderText = cInputBoxEmptyText;
			PageSettings::Init(this->comboBoxOptionsLanguage, this->comboBoxOptionsCellSize, this->comboBoxOptionsMemoryBehavior,
			                   this->comboBoxOptionsEof, this->textBoxOptionsMemorySize, this->textBoxOptionsCommandLine,
							   this->textBoxPhOptionsInput, this->checkBoxOptionRedirectStreams);

			WorkContextBroker::Instance->Initialize(this, tabControlPages);

			//arg processing
			if(argFileToOpen)
			{
				FileContext ^ argOpenFileContext = FileContextFactory::createFileContext(argFileToOpen);
				if(argOpenFileContext->Open()) {
					WorkContextBroker::Instance->OpenPage(argOpenFileContext);
				}
				else
					WorkContextBroker::Instance->AddPage();
			}
			else
				WorkContextBroker::Instance->AddPage();

			//process
			interpreterProcess = nullptr;

			//menus
			uiMenuButtonGenerator::Generate(this->recentMainMenuItem, 
											GlobalOptions::Instance->RecentFilesList, 
											gcnew EventHandler(this, &FormMain::recentMainMenuItem_Click));

			uiMenuButtonGenerator::Generate(this->pluginsMainMenuItem, 
											GlobalOptions::Instance->Plugins->InvocablePluginNames()->ToArray(), 
											gcnew EventHandler(this, &FormMain::pluginsMainMenuItem_Click));

			uiMenuButtonGenerator::Generate(this->editorInsertPluginMenuItem, 
											GlobalOptions::Instance->Plugins->PragmaPluginNames()->ToArray(), 
											gcnew EventHandler(this, &FormMain::editorInsertPluginMenuItem_Click));

			uiMenuRadioButtonGenerator ^ dbgMemRepGenerator = gcnew uiMenuRadioButtonGenerator(this->contextMenuButtonMemoryRepresentation, 
												gcnew array<String^>{cButtonMemoryReprDecCaption,
																	 cButtonMemoryReprCharCaption,
																	 cButtonMemoryReprHexCaption
																	}, 
												gcnew EventHandler(this, &FormMain::contextMenuButtonMemoryRepresentation_Click),
												0);

			uiMenuRadioButtonGenerator ^ dbgColNumGenerator = gcnew uiMenuRadioButtonGenerator(this->contextMenuButtonColumnNumber, 
												gcnew array<String^>{"1","2","5",
																	 "10","15","20",
																	 cButtonMemoryTabColsAutoCaption
																	}, 
												gcnew EventHandler(this, &FormMain::contextMenuButtonChangeMemTabColumnNumber_Click),
												2);

			//tooltips
			uiTooltip::Set( this->textBoxPhOptionsInput, cInputBoxHint );
			uiTooltip::Set( this->buttonDebugStep, cTooltipDebugStepCaption );
			uiTooltip::Set( this->buttonDebugStepOver, cTooltipDebugStepOverCaption );
			uiTooltip::Set( this->buttonDebugRunToCursor, cTooltipDebugRunToCursorCaption );
			uiTooltip::Set( this->buttonDebugPlaceCursor, cTooltipDebugPlaceCursorCaption );
			uiTooltip::Set( this->buttonDebugStop, cTooltipDebugStopCaption );
		
			
			//bevels
			uiHorizontallyAlignedBevel ^ debugPanelBevel = gcnew uiHorizontallyAlignedBevel(panelDebug, 15, 55, 30);


			//apply
			this->mainForm_ToggleInterface(this->interpreterProcess, true);
			this->mainForm_UpdateUI();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormMain()
		{
			if (components)
			{
				delete components;
			}
		}

	private: 
		 InterpreterProcess ^ interpreterProcess;

	protected: 

		//constans
		literal String ^ cButtonAdvancedOptionsShowCaption = "Show advanced options";
		literal String ^ cButtonAdvancedOptionsHideCaption = "Hide advanced options";	
		literal String ^ cInputBoxEmptyText = "Write your input here. Newline stands for ENTER";
		literal String ^ cInputBoxHint = "Write {number} to get any character, {eof} or {nl}.";
		literal String ^ cLogOpenDialogFilters = "Log files|*.log;*.txt|All files|*.*";
		literal String ^ cButtonMemoryReprDecCaption = "Number";
		literal String ^ cButtonMemoryReprCharCaption = "Character";
		literal String ^ cButtonMemoryReprHexCaption = "Hexadecimal";
		literal String ^ cButtonMemoryTabColsAutoCaption = "Fit to width";

		literal String ^ cTooltipDebugStepCaption = "Step one instruction";
		literal String ^ cTooltipDebugCodeCaption = "Run debugger (step by step execution)";
		literal String ^ cTooltipDebugStepOverCaption = "Step over next instruction";
		literal String ^ cTooltipDebugRunToCursorCaption = "Run to instruction at cursor";
		literal String ^ cTooltipDebugPlaceCursorCaption = "Place cursor at current instruction";
		literal String ^ cTooltipDebugStopCaption = "Terminate all";

		literal String ^ cDebugLabelInfo = "Click Step to start debugging";
		literal String ^ cDebugLabelBusy = "Debugger is running on the other tab";
		literal String ^ cDebugLabelInfoDisabled = "Chosen interpreter does not support debugging";
	
		/// <summary>
		/// Required designer variable.
		/// </summary>

		private: System::Windows::Forms::StatusStrip^  statusBar;
		private: System::Windows::Forms::MenuStrip^  mainMenu;

		private: System::Windows::Forms::ToolStrip^  toolBar;
		private: System::Windows::Forms::SplitContainer^  splitContainerMain;
		private: System::Windows::Forms::ToolStripMenuItem^  fileMainMenuItem;
		private: System::Windows::Forms::SplitContainer^  splitContainerEditor;
		private: System::Windows::Forms::TabControl^  tabControlPages;
		private: System::Windows::Forms::ToolStripMenuItem^  newfileMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  openfileMainMenuItem;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem4;
		private: System::Windows::Forms::ToolStripMenuItem^  savefileMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  saveAsMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  saveAsProjectMainMenuItem;

		private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem5;
		private: System::Windows::Forms::ToolStripMenuItem^  closeMainMenuItem;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem6;
		private: System::Windows::Forms::ToolStripMenuItem^  exitMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  editMainMenuItem;
		public: System::Windows::Forms::ContextMenuStrip^  editorContextMenu;

		private: System::Windows::Forms::ToolStripMenuItem^  editorUndoMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  editorRedoMenuItem;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
		private: System::Windows::Forms::ToolStripMenuItem^  editorCutMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  editorCopyMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  editorPasteMenuItem;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
		private: System::Windows::Forms::ToolStripMenuItem^  editorSelectallMenuItem;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem3;
		private: System::Windows::Forms::ToolStripMenuItem^  editorInsertMenuItem;
		private: System::Windows::Forms::TabControl^  tabControlRight;
		private: System::Windows::Forms::TabPage^  tabPageProperties;
		private: System::Windows::Forms::TabPage^  tabPageDebug;
		private: System::Windows::Forms::GroupBox^  groupBoxOptions;
		private: System::Windows::Forms::GroupBox^  groupBoxLanguage;
		public: System::Windows::Forms::ComboBox^  comboBoxOptionsLanguage;
		private: System::Windows::Forms::Label^  labelOptionsEof;
		private: System::Windows::Forms::Label^  labelOptionsMemoryBehavior;
		private: System::Windows::Forms::Label^  labelOptionsCellSize;
		private: System::Windows::Forms::TextBox^  textBoxOptionsMemorySize;
		private: System::Windows::Forms::Label^  labelOptionsMemorySize;
		private: System::Windows::Forms::ComboBox^  comboBoxOptionsEof;
		private: System::Windows::Forms::ComboBox^  comboBoxOptionsMemoryBehavior;
		private: System::Windows::Forms::ComboBox^  comboBoxOptionsCellSize;
		private: System::Windows::Forms::GroupBox^  groupBoxAdvanced;
		private: System::Windows::Forms::Button^  buttonOptionsAdvanced;
		private: System::Windows::Forms::Button^  buttonOptionsDefault;
		private: System::Windows::Forms::Label^  labelOptionsCommandLine;
		private: System::Windows::Forms::TextBox^  textBoxOptionsCommandLine;
		private: System::Windows::Forms::ToolStripMenuItem^  viewMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  propertiesViewMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  outputViewMainMenuItem;
		private: System::Windows::Forms::ToolStripButton^  toolBarButtonNew;

		private: System::ComponentModel::IContainer^  components;

		private: System::Windows::Forms::ToolStripButton^  toolBarButtonOpen;
		private: System::Windows::Forms::ToolStripButton^  toolBarButtonSave;
		private: System::Windows::Forms::ToolStripButton^  toolBarButtonSaveAsProject;

		private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
		private: System::Windows::Forms::ToolStripButton^  toolBarButtonCut;
		private: System::Windows::Forms::ToolStripButton^  toolBarButtonCopy;
		private: System::Windows::Forms::ToolStripButton^  toolBarButtonPaste;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
		private: System::Windows::Forms::ToolStripButton^  toolBarButtonUndo;
		private: System::Windows::Forms::ToolStripButton^  toolBarButtonRedo;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
private: System::Windows::Forms::ToolStripButton^  toolBarButtonDebugStep;

		private: System::Windows::Forms::ToolStripMenuItem^  codeMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  runMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  runSelectionMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  parseMainMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  analyseMainMenuItem;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
		private: System::Windows::Forms::ToolStripMenuItem^  stopMainMenuItem;
		 
		private: System::Windows::Forms::ToolStripMenuItem^  runSeparateMainMenuItem;
		private: System::Windows::Forms::TabControl^  tabControlOutput;
		private: System::Windows::Forms::TabPage^  tabPageErrorList;

		private: System::Windows::Forms::TabPage^  tabPageOutputList;
		public: System::Windows::Forms::ListView^  listViewOutputErrors;

		private: System::Windows::Forms::ColumnHeader^  listViewOutputerrorsColumn1;
		private: System::Windows::Forms::ColumnHeader^  listViewOutputErrorsColumn2;
		private: System::Windows::Forms::ColumnHeader^  listViewOutputErrorsColumn3;
		public: System::Windows::Forms::TextBox^  textBoxOutput;
		private: System::Windows::Forms::GroupBox^  groupBoxIOOptions;
		private: System::Windows::Forms::CheckBox^  checkBoxOptionRedirectStreams;

		private: System::Windows::Forms::ToolStripMenuItem^  optionsMainMenuItem;
		private: System::Windows::Forms::TabPage^  tabPageLog;
		public: System::Windows::Forms::TextBox^  textBoxLog;
		private: TextBoxPlaceholder::TextBoxPlaceholderControl^  textBoxPhOptionsInput;
		private: System::Windows::Forms::ContextMenuStrip^  logTabContextMenu;
		private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonLogRefresh;
		private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonLogOpen;


		private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
		private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonTrackChanges;
		private: System::Windows::Forms::ToolStripMenuItem^  editorInsertConstantMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  debugMainMenuItem;


		private: System::Windows::Forms::Button^  buttonDebugRunToCursor;
		private: System::Windows::Forms::Button^  buttonDebugStepOver;
		private: System::Windows::Forms::Button^  buttonDebugStep;
		private: System::Windows::Forms::ToolStripMenuItem^  recentMainMenuItem;
		private: System::Windows::Forms::ContextMenuStrip^  debugMemViewContextMenu;
		private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonScrollToMemPos;
		private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonMemoryRepresentation;
private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonScrollToLNCell;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
private: System::Windows::Forms::SplitContainer^  splitContainerDebugTools;
private: System::Windows::Forms::TreeView^  treeViewDebugThreadList;
private: System::Windows::Forms::DataGridView^  dataGridViewDebugMemory;
private: System::Windows::Forms::ComboBox^  comboBoxThreadMemSelect;
private: System::Windows::Forms::Panel^  panelDebug;


private: System::Windows::Forms::ContextMenuStrip^  threadContextMenu;
private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonExpandAll;
private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonCollapseAll;
private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonCollapseTree;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonTrackThread;
private: System::Windows::Forms::Label^  labelDebugStatus;
private: System::Windows::Forms::Button^  buttonDebugPlaceCursor;

private: System::Windows::Forms::ToolStripStatusLabel^  statusStripStatusLabel2;
private: System::Windows::Forms::ToolStripButton^  toolBarButtonStepOver;
private: System::Windows::Forms::ToolStripButton^  toolBarButtonRunToCursor;

private: System::Windows::Forms::ToolStripStatusLabel^  statusStripStatusLabel3;
private: System::Windows::Forms::ToolStripStatusLabel^  statusStripStatusLabel4;
private: System::Windows::Forms::ToolStripStatusLabel^  statusStripStatusLabel5;
private: System::Windows::Forms::ToolStripStatusLabel^  statusStripStatusLabel1;
private: System::Windows::Forms::ToolStripMenuItem^  pluginsMainMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  helpMainMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  detachMainMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  editorRestoreMenuItem;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator8;
private: System::Windows::Forms::ToolStripMenuItem^  editorInsertPluginMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  saveAllMainMenuItem;
private: System::Windows::Forms::ToolStripButton^  toolBarButtonSaveAll;
private: System::Windows::Forms::ToolStripButton^  toolBarButtonRunCode;
private: System::Windows::Forms::Button^  buttonDebugStop;
private: System::Windows::Forms::ToolStripMenuItem^  aboutMainMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  aboutBTMainMenuItem;
private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem7;
private: System::Windows::Forms::ToolStripMenuItem^  protipMainMenuItem;
private: System::Windows::Forms::ToolStripStatusLabel^  statusStripStatusLabel6;


		private: System::Windows::Forms::ToolStripMenuItem^  contextMenuButtonColumnNumber;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
			this->editMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editorContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->editorUndoMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editorRedoMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->editorCutMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editorCopyMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editorPasteMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->editorSelectallMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editorRestoreMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->editorInsertMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editorInsertConstantMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator8 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->editorInsertPluginMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusBar = (gcnew System::Windows::Forms::StatusStrip());
			this->statusStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->statusStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->statusStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->statusStripStatusLabel4 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->statusStripStatusLabel5 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->statusStripStatusLabel6 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->mainMenu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newfileMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openfileMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->savefileMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAllMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsProjectMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->recentMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->closeMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->propertiesViewMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->outputViewMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->codeMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->parseMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->analyseMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->debugMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->runMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->stopMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->detachMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->runSelectionMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->runSeparateMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->optionsMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pluginsMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutBTMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->protipMainMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolBar = (gcnew System::Windows::Forms::ToolStrip());
			this->toolBarButtonNew = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonOpen = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonSave = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonSaveAll = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonSaveAsProject = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolBarButtonCut = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonCopy = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonPaste = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolBarButtonUndo = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonRedo = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolBarButtonRunCode = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonDebugStep = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonStepOver = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolBarButtonRunToCursor = (gcnew System::Windows::Forms::ToolStripButton());
			this->splitContainerMain = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainerEditor = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControlPages = (gcnew System::Windows::Forms::TabControl());
			this->tabControlOutput = (gcnew System::Windows::Forms::TabControl());
			this->tabPageErrorList = (gcnew System::Windows::Forms::TabPage());
			this->listViewOutputErrors = (gcnew System::Windows::Forms::ListView());
			this->listViewOutputerrorsColumn1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->listViewOutputErrorsColumn2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->listViewOutputErrorsColumn3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->tabPageOutputList = (gcnew System::Windows::Forms::TabPage());
			this->textBoxOutput = (gcnew System::Windows::Forms::TextBox());
			this->tabPageLog = (gcnew System::Windows::Forms::TabPage());
			this->textBoxLog = (gcnew System::Windows::Forms::TextBox());
			this->logTabContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->contextMenuButtonLogRefresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuButtonLogOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->contextMenuButtonTrackChanges = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tabControlRight = (gcnew System::Windows::Forms::TabControl());
			this->tabPageProperties = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxIOOptions = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxPhOptionsInput = (gcnew TextBoxPlaceholder::TextBoxPlaceholderControl());
			this->checkBoxOptionRedirectStreams = (gcnew System::Windows::Forms::CheckBox());
			this->groupBoxAdvanced = (gcnew System::Windows::Forms::GroupBox());
			this->labelOptionsCommandLine = (gcnew System::Windows::Forms::Label());
			this->textBoxOptionsCommandLine = (gcnew System::Windows::Forms::TextBox());
			this->buttonOptionsAdvanced = (gcnew System::Windows::Forms::Button());
			this->buttonOptionsDefault = (gcnew System::Windows::Forms::Button());
			this->groupBoxLanguage = (gcnew System::Windows::Forms::GroupBox());
			this->comboBoxOptionsLanguage = (gcnew System::Windows::Forms::ComboBox());
			this->groupBoxOptions = (gcnew System::Windows::Forms::GroupBox());
			this->comboBoxOptionsEof = (gcnew System::Windows::Forms::ComboBox());
			this->comboBoxOptionsMemoryBehavior = (gcnew System::Windows::Forms::ComboBox());
			this->comboBoxOptionsCellSize = (gcnew System::Windows::Forms::ComboBox());
			this->labelOptionsEof = (gcnew System::Windows::Forms::Label());
			this->labelOptionsMemoryBehavior = (gcnew System::Windows::Forms::Label());
			this->labelOptionsCellSize = (gcnew System::Windows::Forms::Label());
			this->textBoxOptionsMemorySize = (gcnew System::Windows::Forms::TextBox());
			this->labelOptionsMemorySize = (gcnew System::Windows::Forms::Label());
			this->tabPageDebug = (gcnew System::Windows::Forms::TabPage());
			this->panelDebug = (gcnew System::Windows::Forms::Panel());
			this->buttonDebugStop = (gcnew System::Windows::Forms::Button());
			this->labelDebugStatus = (gcnew System::Windows::Forms::Label());
			this->buttonDebugPlaceCursor = (gcnew System::Windows::Forms::Button());
			this->buttonDebugStep = (gcnew System::Windows::Forms::Button());
			this->buttonDebugStepOver = (gcnew System::Windows::Forms::Button());
			this->buttonDebugRunToCursor = (gcnew System::Windows::Forms::Button());
			this->splitContainerDebugTools = (gcnew System::Windows::Forms::SplitContainer());
			this->treeViewDebugThreadList = (gcnew System::Windows::Forms::TreeView());
			this->threadContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->contextMenuButtonTrackThread = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->contextMenuButtonExpandAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuButtonCollapseAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuButtonCollapseTree = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->comboBoxThreadMemSelect = (gcnew System::Windows::Forms::ComboBox());
			this->dataGridViewDebugMemory = (gcnew System::Windows::Forms::DataGridView());
			this->debugMemViewContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->contextMenuButtonScrollToMemPos = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuButtonScrollToLNCell = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->contextMenuButtonMemoryRepresentation = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuButtonColumnNumber = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editorContextMenu->SuspendLayout();
			this->statusBar->SuspendLayout();
			this->mainMenu->SuspendLayout();
			this->toolBar->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerMain))->BeginInit();
			this->splitContainerMain->Panel1->SuspendLayout();
			this->splitContainerMain->Panel2->SuspendLayout();
			this->splitContainerMain->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerEditor))->BeginInit();
			this->splitContainerEditor->Panel1->SuspendLayout();
			this->splitContainerEditor->Panel2->SuspendLayout();
			this->splitContainerEditor->SuspendLayout();
			this->tabControlOutput->SuspendLayout();
			this->tabPageErrorList->SuspendLayout();
			this->tabPageOutputList->SuspendLayout();
			this->tabPageLog->SuspendLayout();
			this->logTabContextMenu->SuspendLayout();
			this->tabControlRight->SuspendLayout();
			this->tabPageProperties->SuspendLayout();
			this->groupBoxIOOptions->SuspendLayout();
			this->groupBoxAdvanced->SuspendLayout();
			this->groupBoxLanguage->SuspendLayout();
			this->groupBoxOptions->SuspendLayout();
			this->tabPageDebug->SuspendLayout();
			this->panelDebug->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerDebugTools))->BeginInit();
			this->splitContainerDebugTools->Panel1->SuspendLayout();
			this->splitContainerDebugTools->Panel2->SuspendLayout();
			this->splitContainerDebugTools->SuspendLayout();
			this->threadContextMenu->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewDebugMemory))->BeginInit();
			this->debugMemViewContextMenu->SuspendLayout();
			this->SuspendLayout();
			// 
			// editMainMenuItem
			// 
			this->editMainMenuItem->DropDown = this->editorContextMenu;
			this->editMainMenuItem->Name = L"editMainMenuItem";
			this->editMainMenuItem->Size = System::Drawing::Size(39, 20);
			this->editMainMenuItem->Text = L"Edit";
			// 
			// editorContextMenu
			// 
			this->editorContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {this->editorUndoMenuItem, 
				this->editorRedoMenuItem, this->toolStripMenuItem1, this->editorCutMenuItem, this->editorCopyMenuItem, this->editorPasteMenuItem, 
				this->toolStripMenuItem2, this->editorSelectallMenuItem, this->editorRestoreMenuItem, this->toolStripMenuItem3, this->editorInsertMenuItem});
			this->editorContextMenu->Name = L"editorContextMenu";
			this->editorContextMenu->OwnerItem = this->editMainMenuItem;
			this->editorContextMenu->Size = System::Drawing::Size(217, 198);
			// 
			// editorUndoMenuItem
			// 
			this->editorUndoMenuItem->Enabled = false;
			this->editorUndoMenuItem->ForeColor = System::Drawing::SystemColors::ControlText;
			this->editorUndoMenuItem->Name = L"editorUndoMenuItem";
			this->editorUndoMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Z));
			this->editorUndoMenuItem->Size = System::Drawing::Size(216, 22);
			this->editorUndoMenuItem->Text = L"Undo";
			this->editorUndoMenuItem->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Undo);
			// 
			// editorRedoMenuItem
			// 
			this->editorRedoMenuItem->Enabled = false;
			this->editorRedoMenuItem->Name = L"editorRedoMenuItem";
			this->editorRedoMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Y));
			this->editorRedoMenuItem->Size = System::Drawing::Size(216, 22);
			this->editorRedoMenuItem->Text = L"Redo";
			this->editorRedoMenuItem->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Redo);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(213, 6);
			// 
			// editorCutMenuItem
			// 
			this->editorCutMenuItem->Name = L"editorCutMenuItem";
			this->editorCutMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::X));
			this->editorCutMenuItem->Size = System::Drawing::Size(216, 22);
			this->editorCutMenuItem->Text = L"Cut";
			this->editorCutMenuItem->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Cut);
			// 
			// editorCopyMenuItem
			// 
			this->editorCopyMenuItem->Name = L"editorCopyMenuItem";
			this->editorCopyMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::C));
			this->editorCopyMenuItem->Size = System::Drawing::Size(216, 22);
			this->editorCopyMenuItem->Text = L"Copy";
			this->editorCopyMenuItem->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Copy);
			// 
			// editorPasteMenuItem
			// 
			this->editorPasteMenuItem->Name = L"editorPasteMenuItem";
			this->editorPasteMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::V));
			this->editorPasteMenuItem->Size = System::Drawing::Size(216, 22);
			this->editorPasteMenuItem->Text = L"Paste";
			this->editorPasteMenuItem->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Paste);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(213, 6);
			// 
			// editorSelectallMenuItem
			// 
			this->editorSelectallMenuItem->Name = L"editorSelectallMenuItem";
			this->editorSelectallMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::A));
			this->editorSelectallMenuItem->Size = System::Drawing::Size(216, 22);
			this->editorSelectallMenuItem->Text = L"Select all";
			this->editorSelectallMenuItem->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_SelectAll);
			// 
			// editorRestoreMenuItem
			// 
			this->editorRestoreMenuItem->Enabled = false;
			this->editorRestoreMenuItem->Name = L"editorRestoreMenuItem";
			this->editorRestoreMenuItem->Size = System::Drawing::Size(216, 22);
			this->editorRestoreMenuItem->Text = L"Restore to previous version";
			this->editorRestoreMenuItem->Click += gcnew System::EventHandler(this, &FormMain::editorRestoreMenuItem_Click);
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(213, 6);
			// 
			// editorInsertMenuItem
			// 
			this->editorInsertMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->editorInsertConstantMenuItem, 
				this->toolStripSeparator8, this->editorInsertPluginMenuItem});
			this->editorInsertMenuItem->Name = L"editorInsertMenuItem";
			this->editorInsertMenuItem->Size = System::Drawing::Size(216, 22);
			this->editorInsertMenuItem->Text = L"Insert";
			// 
			// editorInsertConstantMenuItem
			// 
			this->editorInsertConstantMenuItem->Name = L"editorInsertConstantMenuItem";
			this->editorInsertConstantMenuItem->Size = System::Drawing::Size(122, 22);
			this->editorInsertConstantMenuItem->Text = L"Constant";
			this->editorInsertConstantMenuItem->Click += gcnew System::EventHandler(this, &FormMain::editorInsertConstantMenuItem_Click);
			// 
			// toolStripSeparator8
			// 
			this->toolStripSeparator8->Name = L"toolStripSeparator8";
			this->toolStripSeparator8->Size = System::Drawing::Size(119, 6);
			// 
			// editorInsertPluginMenuItem
			// 
			this->editorInsertPluginMenuItem->Name = L"editorInsertPluginMenuItem";
			this->editorInsertPluginMenuItem->Size = System::Drawing::Size(122, 22);
			this->editorInsertPluginMenuItem->Text = L"Pragma";
			// 
			// statusBar
			// 
			this->statusBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->statusStripStatusLabel1, 
				this->statusStripStatusLabel2, this->statusStripStatusLabel3, this->statusStripStatusLabel4, this->statusStripStatusLabel5, this->statusStripStatusLabel6});
			this->statusBar->Location = System::Drawing::Point(0, 756);
			this->statusBar->Name = L"statusBar";
			this->statusBar->Size = System::Drawing::Size(1110, 22);
			this->statusBar->TabIndex = 4;
			// 
			// statusStripStatusLabel1
			// 
			this->statusStripStatusLabel1->AutoSize = false;
			this->statusStripStatusLabel1->BorderStyle = System::Windows::Forms::Border3DStyle::SunkenOuter;
			this->statusStripStatusLabel1->Name = L"statusStripStatusLabel1";
			this->statusStripStatusLabel1->Size = System::Drawing::Size(120, 17);
			this->statusStripStatusLabel1->Text = L"file";
			this->statusStripStatusLabel1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// statusStripStatusLabel2
			// 
			this->statusStripStatusLabel2->AutoSize = false;
			this->statusStripStatusLabel2->BorderStyle = System::Windows::Forms::Border3DStyle::Sunken;
			this->statusStripStatusLabel2->Name = L"statusStripStatusLabel2";
			this->statusStripStatusLabel2->Size = System::Drawing::Size(300, 17);
			this->statusStripStatusLabel2->Text = L"state";
			// 
			// statusStripStatusLabel3
			// 
			this->statusStripStatusLabel3->AutoSize = false;
			this->statusStripStatusLabel3->BorderStyle = System::Windows::Forms::Border3DStyle::Sunken;
			this->statusStripStatusLabel3->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->statusStripStatusLabel3->Name = L"statusStripStatusLabel3";
			this->statusStripStatusLabel3->Size = System::Drawing::Size(350, 17);
			this->statusStripStatusLabel3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// statusStripStatusLabel4
			// 
			this->statusStripStatusLabel4->AutoSize = false;
			this->statusStripStatusLabel4->BorderStyle = System::Windows::Forms::Border3DStyle::Etched;
			this->statusStripStatusLabel4->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->statusStripStatusLabel4->Name = L"statusStripStatusLabel4";
			this->statusStripStatusLabel4->Size = System::Drawing::Size(100, 17);
			this->statusStripStatusLabel4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// statusStripStatusLabel5
			// 
			this->statusStripStatusLabel5->AutoSize = false;
			this->statusStripStatusLabel5->BorderStyle = System::Windows::Forms::Border3DStyle::Sunken;
			this->statusStripStatusLabel5->ImageAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->statusStripStatusLabel5->Name = L"statusStripStatusLabel5";
			this->statusStripStatusLabel5->Overflow = System::Windows::Forms::ToolStripItemOverflow::Always;
			this->statusStripStatusLabel5->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->statusStripStatusLabel5->Size = System::Drawing::Size(100, 17);
			// 
			// statusStripStatusLabel6
			// 
			this->statusStripStatusLabel6->Name = L"statusStripStatusLabel6";
			this->statusStripStatusLabel6->Size = System::Drawing::Size(125, 17);
			this->statusStripStatusLabel6->Spring = true;
			this->statusStripStatusLabel6->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// mainMenu
			// 
			this->mainMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->fileMainMenuItem, this->editMainMenuItem, 
				this->viewMainMenuItem, this->codeMainMenuItem, this->optionsMainMenuItem, this->pluginsMainMenuItem, this->helpMainMenuItem});
			this->mainMenu->Location = System::Drawing::Point(0, 0);
			this->mainMenu->Name = L"mainMenu";
			this->mainMenu->Size = System::Drawing::Size(1110, 24);
			this->mainMenu->TabIndex = 5;
			this->mainMenu->Text = L"menuStrip1";
			// 
			// fileMainMenuItem
			// 
			this->fileMainMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {this->newfileMainMenuItem, 
				this->openfileMainMenuItem, this->toolStripMenuItem4, this->savefileMainMenuItem, this->saveAsMainMenuItem, this->saveAllMainMenuItem, 
				this->saveAsProjectMainMenuItem, this->toolStripMenuItem5, this->recentMainMenuItem, this->closeMainMenuItem, this->toolStripMenuItem6, 
				this->exitMainMenuItem});
			this->fileMainMenuItem->Name = L"fileMainMenuItem";
			this->fileMainMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileMainMenuItem->Text = L"File";
			// 
			// newfileMainMenuItem
			// 
			this->newfileMainMenuItem->Name = L"newfileMainMenuItem";
			this->newfileMainMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
			this->newfileMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->newfileMainMenuItem->Text = L"&New";
			this->newfileMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::newfileMainMenuItem_Click);
			// 
			// openfileMainMenuItem
			// 
			this->openfileMainMenuItem->Name = L"openfileMainMenuItem";
			this->openfileMainMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->openfileMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->openfileMainMenuItem->Text = L"&Open";
			this->openfileMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::openfileMainMenuItem_Click);
			// 
			// toolStripMenuItem4
			// 
			this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
			this->toolStripMenuItem4->Size = System::Drawing::Size(212, 6);
			// 
			// savefileMainMenuItem
			// 
			this->savefileMainMenuItem->Name = L"savefileMainMenuItem";
			this->savefileMainMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->savefileMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->savefileMainMenuItem->Text = L"&Save";
			this->savefileMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::savefileMainMenuItem_Click);
			// 
			// saveAsMainMenuItem
			// 
			this->saveAsMainMenuItem->Name = L"saveAsMainMenuItem";
			this->saveAsMainMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift) 
				| System::Windows::Forms::Keys::S));
			this->saveAsMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->saveAsMainMenuItem->Text = L"Sa&ve as";
			this->saveAsMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::saveAsMainMenuItem_Click);
			// 
			// saveAllMainMenuItem
			// 
			this->saveAllMainMenuItem->Name = L"saveAllMainMenuItem";
			this->saveAllMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->saveAllMainMenuItem->Text = L"Save &all";
			this->saveAllMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::saveAllMainMenuItem_Click);
			// 
			// saveAsProjectMainMenuItem
			// 
			this->saveAsProjectMainMenuItem->Name = L"saveAsProjectMainMenuItem";
			this->saveAsProjectMainMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Alt) 
				| System::Windows::Forms::Keys::S));
			this->saveAsProjectMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->saveAsProjectMainMenuItem->Text = L"Save as &project";
			this->saveAsProjectMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::saveAsProjectMainMenuItem_Click);
			// 
			// toolStripMenuItem5
			// 
			this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
			this->toolStripMenuItem5->Size = System::Drawing::Size(212, 6);
			// 
			// recentMainMenuItem
			// 
			this->recentMainMenuItem->Name = L"recentMainMenuItem";
			this->recentMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->recentMainMenuItem->Text = L"&Recent";
			// 
			// closeMainMenuItem
			// 
			this->closeMainMenuItem->Name = L"closeMainMenuItem";
			this->closeMainMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::W));
			this->closeMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->closeMainMenuItem->Text = L"&Close";
			this->closeMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::closeMainMenuItem_Click);
			// 
			// toolStripMenuItem6
			// 
			this->toolStripMenuItem6->Name = L"toolStripMenuItem6";
			this->toolStripMenuItem6->Size = System::Drawing::Size(212, 6);
			// 
			// exitMainMenuItem
			// 
			this->exitMainMenuItem->Name = L"exitMainMenuItem";
			this->exitMainMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::F4));
			this->exitMainMenuItem->Size = System::Drawing::Size(215, 22);
			this->exitMainMenuItem->Text = L"&Exit";
			// 
			// viewMainMenuItem
			// 
			this->viewMainMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->propertiesViewMainMenuItem, 
				this->outputViewMainMenuItem});
			this->viewMainMenuItem->Name = L"viewMainMenuItem";
			this->viewMainMenuItem->Size = System::Drawing::Size(44, 20);
			this->viewMainMenuItem->Text = L"View";
			// 
			// propertiesViewMainMenuItem
			// 
			this->propertiesViewMainMenuItem->Checked = true;
			this->propertiesViewMainMenuItem->CheckOnClick = true;
			this->propertiesViewMainMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->propertiesViewMainMenuItem->Name = L"propertiesViewMainMenuItem";
			this->propertiesViewMainMenuItem->Size = System::Drawing::Size(159, 22);
			this->propertiesViewMainMenuItem->Text = L"Properties panel";
			this->propertiesViewMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::propertiesViewMainMenuItem_Click);
			// 
			// outputViewMainMenuItem
			// 
			this->outputViewMainMenuItem->Checked = true;
			this->outputViewMainMenuItem->CheckOnClick = true;
			this->outputViewMainMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->outputViewMainMenuItem->Name = L"outputViewMainMenuItem";
			this->outputViewMainMenuItem->Size = System::Drawing::Size(159, 22);
			this->outputViewMainMenuItem->Text = L"Output panel";
			this->outputViewMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::propertiesViewMainMenuItem_Click);
			// 
			// codeMainMenuItem
			// 
			this->codeMainMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->parseMainMenuItem, 
				this->analyseMainMenuItem, this->debugMainMenuItem, this->toolStripSeparator4, this->runMainMenuItem, this->stopMainMenuItem, 
				this->detachMainMenuItem, this->runSelectionMainMenuItem, this->runSeparateMainMenuItem});
			this->codeMainMenuItem->Name = L"codeMainMenuItem";
			this->codeMainMenuItem->Size = System::Drawing::Size(47, 20);
			this->codeMainMenuItem->Text = L"Code";
			// 
			// parseMainMenuItem
			// 
			this->parseMainMenuItem->Name = L"parseMainMenuItem";
			this->parseMainMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F11;
			this->parseMainMenuItem->Size = System::Drawing::Size(192, 22);
			this->parseMainMenuItem->Text = L"Parse";
			this->parseMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::parseMainMenuItem_Click);
			// 
			// analyseMainMenuItem
			// 
			this->analyseMainMenuItem->Name = L"analyseMainMenuItem";
			this->analyseMainMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F10;
			this->analyseMainMenuItem->Size = System::Drawing::Size(192, 22);
			this->analyseMainMenuItem->Text = L"Analyse";
			this->analyseMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::analyseMainMenuItem_Click);
			// 
			// debugMainMenuItem
			// 
			this->debugMainMenuItem->Name = L"debugMainMenuItem";
			this->debugMainMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F5;
			this->debugMainMenuItem->Size = System::Drawing::Size(192, 22);
			this->debugMainMenuItem->Text = L"Debug";
			this->debugMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::debugMainMenuItem_Click);
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(189, 6);
			// 
			// runMainMenuItem
			// 
			this->runMainMenuItem->Name = L"runMainMenuItem";
			this->runMainMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F7;
			this->runMainMenuItem->Size = System::Drawing::Size(192, 22);
			this->runMainMenuItem->Text = L"Run";
			this->runMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::runMainMenuItem_Click);
			// 
			// stopMainMenuItem
			// 
			this->stopMainMenuItem->Name = L"stopMainMenuItem";
			this->stopMainMenuItem->Size = System::Drawing::Size(192, 22);
			this->stopMainMenuItem->Text = L"Stop";
			this->stopMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::stopMainMenuItem_Click);
			// 
			// detachMainMenuItem
			// 
			this->detachMainMenuItem->Name = L"detachMainMenuItem";
			this->detachMainMenuItem->Size = System::Drawing::Size(192, 22);
			this->detachMainMenuItem->Text = L"Detach";
			this->detachMainMenuItem->Visible = false;
			this->detachMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::detachMainMenuItem_Click);
			// 
			// runSelectionMainMenuItem
			// 
			this->runSelectionMainMenuItem->Name = L"runSelectionMainMenuItem";
			this->runSelectionMainMenuItem->Size = System::Drawing::Size(192, 22);
			this->runSelectionMainMenuItem->Text = L"Run selection";
			this->runSelectionMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::runSelectionMainMenuItem_Click);
			// 
			// runSeparateMainMenuItem
			// 
			this->runSeparateMainMenuItem->Name = L"runSeparateMainMenuItem";
			this->runSeparateMainMenuItem->Size = System::Drawing::Size(192, 22);
			this->runSeparateMainMenuItem->Text = L"Run in separate thread";
			this->runSeparateMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::runSeparateMainMenuItem_Click);
			// 
			// optionsMainMenuItem
			// 
			this->optionsMainMenuItem->Name = L"optionsMainMenuItem";
			this->optionsMainMenuItem->Size = System::Drawing::Size(61, 20);
			this->optionsMainMenuItem->Text = L"Options";
			this->optionsMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::optionsMainMenuItem_Click);
			// 
			// pluginsMainMenuItem
			// 
			this->pluginsMainMenuItem->Name = L"pluginsMainMenuItem";
			this->pluginsMainMenuItem->Size = System::Drawing::Size(58, 20);
			this->pluginsMainMenuItem->Text = L"Plugins";
			// 
			// helpMainMenuItem
			// 
			this->helpMainMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->aboutMainMenuItem, 
				this->aboutBTMainMenuItem, this->toolStripMenuItem7, this->protipMainMenuItem});
			this->helpMainMenuItem->Name = L"helpMainMenuItem";
			this->helpMainMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpMainMenuItem->Text = L"Help";
			// 
			// aboutMainMenuItem
			// 
			this->aboutMainMenuItem->Name = L"aboutMainMenuItem";
			this->aboutMainMenuItem->Size = System::Drawing::Size(223, 22);
			this->aboutMainMenuItem->Text = L"About";
			this->aboutMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::aboutMainMenuItem_Click);
			// 
			// aboutBTMainMenuItem
			// 
			this->aboutBTMainMenuItem->Name = L"aboutBTMainMenuItem";
			this->aboutBTMainMenuItem->Size = System::Drawing::Size(223, 22);
			this->aboutBTMainMenuItem->Text = L"About Brainthread language";
			this->aboutBTMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::protipMainMenuItem_Click);
			// 
			// toolStripMenuItem7
			// 
			this->toolStripMenuItem7->Name = L"toolStripMenuItem7";
			this->toolStripMenuItem7->Size = System::Drawing::Size(220, 6);
			// 
			// protipMainMenuItem
			// 
			this->protipMainMenuItem->Name = L"protipMainMenuItem";
			this->protipMainMenuItem->Size = System::Drawing::Size(223, 22);
			this->protipMainMenuItem->Text = L"Protip";
			this->protipMainMenuItem->Click += gcnew System::EventHandler(this, &FormMain::protipMainMenuItem_Click);
			// 
			// toolBar
			// 
			this->toolBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(17) {this->toolBarButtonNew, this->toolBarButtonOpen, 
				this->toolBarButtonSave, this->toolBarButtonSaveAll, this->toolBarButtonSaveAsProject, this->toolStripSeparator1, this->toolBarButtonCut, 
				this->toolBarButtonCopy, this->toolBarButtonPaste, this->toolStripSeparator2, this->toolBarButtonUndo, this->toolBarButtonRedo, 
				this->toolStripSeparator3, this->toolBarButtonRunCode, this->toolBarButtonDebugStep, this->toolBarButtonStepOver, this->toolBarButtonRunToCursor});
			this->toolBar->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
			this->toolBar->Location = System::Drawing::Point(0, 24);
			this->toolBar->Name = L"toolBar";
			this->toolBar->Size = System::Drawing::Size(1110, 25);
			this->toolBar->TabIndex = 6;
			this->toolBar->Text = L"toolStrip1";
			// 
			// toolBarButtonNew
			// 
			this->toolBarButtonNew->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonNew->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonNew.Image")));
			this->toolBarButtonNew->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonNew->Name = L"toolBarButtonNew";
			this->toolBarButtonNew->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonNew->Text = L"Create a new file";
			this->toolBarButtonNew->Click += gcnew System::EventHandler(this, &FormMain::newfileMainMenuItem_Click);
			// 
			// toolBarButtonOpen
			// 
			this->toolBarButtonOpen->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonOpen->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonOpen.Image")));
			this->toolBarButtonOpen->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonOpen->Name = L"toolBarButtonOpen";
			this->toolBarButtonOpen->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonOpen->Text = L"Open existing file";
			this->toolBarButtonOpen->Click += gcnew System::EventHandler(this, &FormMain::openfileMainMenuItem_Click);
			// 
			// toolBarButtonSave
			// 
			this->toolBarButtonSave->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonSave.Image")));
			this->toolBarButtonSave->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonSave->Name = L"toolBarButtonSave";
			this->toolBarButtonSave->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonSave->Text = L"Save current file";
			this->toolBarButtonSave->Click += gcnew System::EventHandler(this, &FormMain::savefileMainMenuItem_Click);
			// 
			// toolBarButtonSaveAll
			// 
			this->toolBarButtonSaveAll->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonSaveAll->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonSaveAll.Image")));
			this->toolBarButtonSaveAll->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonSaveAll->Name = L"toolBarButtonSaveAll";
			this->toolBarButtonSaveAll->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonSaveAll->Text = L"Save all";
			this->toolBarButtonSaveAll->Click += gcnew System::EventHandler(this, &FormMain::saveAllMainMenuItem_Click);
			// 
			// toolBarButtonSaveAsProject
			// 
			this->toolBarButtonSaveAsProject->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonSaveAsProject->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonSaveAsProject.Image")));
			this->toolBarButtonSaveAsProject->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonSaveAsProject->Name = L"toolBarButtonSaveAsProject";
			this->toolBarButtonSaveAsProject->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonSaveAsProject->Text = L"Save as project";
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
			// 
			// toolBarButtonCut
			// 
			this->toolBarButtonCut->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonCut->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonCut.Image")));
			this->toolBarButtonCut->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonCut->Name = L"toolBarButtonCut";
			this->toolBarButtonCut->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonCut->Text = L"Cut";
			this->toolBarButtonCut->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Cut);
			// 
			// toolBarButtonCopy
			// 
			this->toolBarButtonCopy->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonCopy->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonCopy.Image")));
			this->toolBarButtonCopy->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonCopy->Name = L"toolBarButtonCopy";
			this->toolBarButtonCopy->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonCopy->Text = L"Copy";
			this->toolBarButtonCopy->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Copy);
			// 
			// toolBarButtonPaste
			// 
			this->toolBarButtonPaste->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonPaste->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonPaste.Image")));
			this->toolBarButtonPaste->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonPaste->Name = L"toolBarButtonPaste";
			this->toolBarButtonPaste->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonPaste->Text = L"Paste";
			this->toolBarButtonPaste->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Paste);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(6, 25);
			// 
			// toolBarButtonUndo
			// 
			this->toolBarButtonUndo->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonUndo->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonUndo.Image")));
			this->toolBarButtonUndo->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonUndo->Name = L"toolBarButtonUndo";
			this->toolBarButtonUndo->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonUndo->Text = L"Undo";
			this->toolBarButtonUndo->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Undo);
			// 
			// toolBarButtonRedo
			// 
			this->toolBarButtonRedo->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonRedo->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonRedo.Image")));
			this->toolBarButtonRedo->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonRedo->Name = L"toolBarButtonRedo";
			this->toolBarButtonRedo->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonRedo->Text = L"Redo";
			this->toolBarButtonRedo->Click += gcnew System::EventHandler(this, &FormMain::editorRichTextBox_Redo);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(6, 25);
			// 
			// toolBarButtonRunCode
			// 
			this->toolBarButtonRunCode->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonRunCode->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonRunCode.Image")));
			this->toolBarButtonRunCode->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonRunCode->Name = L"toolBarButtonRunCode";
			this->toolBarButtonRunCode->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonRunCode->Text = L"Run code";
			this->toolBarButtonRunCode->Click += gcnew System::EventHandler(this, &FormMain::runMainMenuItem_Click);
			// 
			// toolBarButtonDebugStep
			// 
			this->toolBarButtonDebugStep->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonDebugStep->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonDebugStep.Image")));
			this->toolBarButtonDebugStep->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonDebugStep->Name = L"toolBarButtonDebugStep";
			this->toolBarButtonDebugStep->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonDebugStep->Text = L"Run code";
			this->toolBarButtonDebugStep->ToolTipText = L"Step one instruction";
			this->toolBarButtonDebugStep->Click += gcnew System::EventHandler(this, &FormMain::buttonDebugStep_Click);
			// 
			// toolBarButtonStepOver
			// 
			this->toolBarButtonStepOver->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonStepOver->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonStepOver.Image")));
			this->toolBarButtonStepOver->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonStepOver->Name = L"toolBarButtonStepOver";
			this->toolBarButtonStepOver->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonStepOver->Text = L"Step over instruction";
			this->toolBarButtonStepOver->ToolTipText = L"Step over instruction";
			this->toolBarButtonStepOver->Click += gcnew System::EventHandler(this, &FormMain::buttonDebugStepOver_Click);
			// 
			// toolBarButtonRunToCursor
			// 
			this->toolBarButtonRunToCursor->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolBarButtonRunToCursor->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolBarButtonRunToCursor.Image")));
			this->toolBarButtonRunToCursor->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolBarButtonRunToCursor->Name = L"toolBarButtonRunToCursor";
			this->toolBarButtonRunToCursor->Size = System::Drawing::Size(23, 22);
			this->toolBarButtonRunToCursor->ToolTipText = L"Run to instruction at cursor";
			this->toolBarButtonRunToCursor->Click += gcnew System::EventHandler(this, &FormMain::buttonDebugRunToCursor_Click);
			// 
			// splitContainerMain
			// 
			this->splitContainerMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerMain->Location = System::Drawing::Point(0, 49);
			this->splitContainerMain->Name = L"splitContainerMain";
			// 
			// splitContainerMain.Panel1
			// 
			this->splitContainerMain->Panel1->Controls->Add(this->splitContainerEditor);
			// 
			// splitContainerMain.Panel2
			// 
			this->splitContainerMain->Panel2->Controls->Add(this->tabControlRight);
			this->splitContainerMain->Panel2MinSize = 335;
			this->splitContainerMain->Size = System::Drawing::Size(1110, 707);
			this->splitContainerMain->SplitterDistance = 730;
			this->splitContainerMain->TabIndex = 7;
			// 
			// splitContainerEditor
			// 
			this->splitContainerEditor->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerEditor->Location = System::Drawing::Point(0, 0);
			this->splitContainerEditor->Name = L"splitContainerEditor";
			this->splitContainerEditor->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainerEditor.Panel1
			// 
			this->splitContainerEditor->Panel1->Controls->Add(this->tabControlPages);
			// 
			// splitContainerEditor.Panel2
			// 
			this->splitContainerEditor->Panel2->Controls->Add(this->tabControlOutput);
			this->splitContainerEditor->Size = System::Drawing::Size(730, 707);
			this->splitContainerEditor->SplitterDistance = 514;
			this->splitContainerEditor->TabIndex = 0;
			// 
			// tabControlPages
			// 
			this->tabControlPages->AllowDrop = true;
			this->tabControlPages->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControlPages->Location = System::Drawing::Point(0, 0);
			this->tabControlPages->Name = L"tabControlPages";
			this->tabControlPages->SelectedIndex = 0;
			this->tabControlPages->Size = System::Drawing::Size(730, 514);
			this->tabControlPages->TabIndex = 4;
			this->tabControlPages->Selected += gcnew System::Windows::Forms::TabControlEventHandler(this, &FormMain::tabControlPages_Selected);
			this->tabControlPages->Deselected += gcnew System::Windows::Forms::TabControlEventHandler(this, &FormMain::tabControlPages_Deselected);
			this->tabControlPages->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::tabControlPages_DragDrop);
			this->tabControlPages->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::tabControlPages_DragEnter);
			this->tabControlPages->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::tabControlPages_DragOver);
			this->tabControlPages->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &FormMain::tabControlPages_MouseDown);
			// 
			// tabControlOutput
			// 
			this->tabControlOutput->Alignment = System::Windows::Forms::TabAlignment::Bottom;
			this->tabControlOutput->Controls->Add(this->tabPageErrorList);
			this->tabControlOutput->Controls->Add(this->tabPageOutputList);
			this->tabControlOutput->Controls->Add(this->tabPageLog);
			this->tabControlOutput->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControlOutput->Location = System::Drawing::Point(0, 0);
			this->tabControlOutput->Name = L"tabControlOutput";
			this->tabControlOutput->SelectedIndex = 0;
			this->tabControlOutput->Size = System::Drawing::Size(730, 189);
			this->tabControlOutput->TabIndex = 0;
			// 
			// tabPageErrorList
			// 
			this->tabPageErrorList->Controls->Add(this->listViewOutputErrors);
			this->tabPageErrorList->Location = System::Drawing::Point(4, 4);
			this->tabPageErrorList->Name = L"tabPageErrorList";
			this->tabPageErrorList->Padding = System::Windows::Forms::Padding(3);
			this->tabPageErrorList->Size = System::Drawing::Size(722, 163);
			this->tabPageErrorList->TabIndex = 0;
			this->tabPageErrorList->Text = L"Error List";
			this->tabPageErrorList->UseVisualStyleBackColor = true;
			// 
			// listViewOutputErrors
			// 
			this->listViewOutputErrors->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->listViewOutputErrors->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->listViewOutputerrorsColumn1, 
				this->listViewOutputErrorsColumn2, this->listViewOutputErrorsColumn3});
			this->listViewOutputErrors->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listViewOutputErrors->FullRowSelect = true;
			this->listViewOutputErrors->LabelWrap = false;
			this->listViewOutputErrors->Location = System::Drawing::Point(3, 3);
			this->listViewOutputErrors->MultiSelect = false;
			this->listViewOutputErrors->Name = L"listViewOutputErrors";
			this->listViewOutputErrors->Size = System::Drawing::Size(716, 157);
			this->listViewOutputErrors->TabIndex = 0;
			this->listViewOutputErrors->UseCompatibleStateImageBehavior = false;
			this->listViewOutputErrors->View = System::Windows::Forms::View::Details;
			this->listViewOutputErrors->ColumnWidthChanged += gcnew System::Windows::Forms::ColumnWidthChangedEventHandler(this, &FormMain::listViewOutputErrors_ColumnWidthChanged);
			this->listViewOutputErrors->DoubleClick += gcnew System::EventHandler(this, &FormMain::listViewOutputErrors_DoubleClick);
			// 
			// listViewOutputerrorsColumn1
			// 
			this->listViewOutputerrorsColumn1->Text = L"Kind";
			// 
			// listViewOutputErrorsColumn2
			// 
			this->listViewOutputErrorsColumn2->Text = L"Instruction";
			// 
			// listViewOutputErrorsColumn3
			// 
			this->listViewOutputErrorsColumn3->Text = L"Description";
			// 
			// tabPageOutputList
			// 
			this->tabPageOutputList->Controls->Add(this->textBoxOutput);
			this->tabPageOutputList->Location = System::Drawing::Point(4, 4);
			this->tabPageOutputList->Name = L"tabPageOutputList";
			this->tabPageOutputList->Padding = System::Windows::Forms::Padding(3);
			this->tabPageOutputList->Size = System::Drawing::Size(722, 163);
			this->tabPageOutputList->TabIndex = 1;
			this->tabPageOutputList->Text = L"Output";
			this->tabPageOutputList->UseVisualStyleBackColor = true;
			// 
			// textBoxOutput
			// 
			this->textBoxOutput->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->textBoxOutput->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBoxOutput->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->textBoxOutput->Location = System::Drawing::Point(3, 3);
			this->textBoxOutput->Multiline = true;
			this->textBoxOutput->Name = L"textBoxOutput";
			this->textBoxOutput->ReadOnly = true;
			this->textBoxOutput->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxOutput->Size = System::Drawing::Size(716, 157);
			this->textBoxOutput->TabIndex = 2;
			this->textBoxOutput->TextChanged += gcnew System::EventHandler(this, &FormMain::textBoxOutput_TextChanged);
			// 
			// tabPageLog
			// 
			this->tabPageLog->Controls->Add(this->textBoxLog);
			this->tabPageLog->Location = System::Drawing::Point(4, 4);
			this->tabPageLog->Name = L"tabPageLog";
			this->tabPageLog->Padding = System::Windows::Forms::Padding(3);
			this->tabPageLog->Size = System::Drawing::Size(722, 163);
			this->tabPageLog->TabIndex = 2;
			this->tabPageLog->Text = L"Log";
			this->tabPageLog->UseVisualStyleBackColor = true;
			// 
			// textBoxLog
			// 
			this->textBoxLog->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->textBoxLog->ContextMenuStrip = this->logTabContextMenu;
			this->textBoxLog->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBoxLog->Location = System::Drawing::Point(3, 3);
			this->textBoxLog->Multiline = true;
			this->textBoxLog->Name = L"textBoxLog";
			this->textBoxLog->ReadOnly = true;
			this->textBoxLog->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxLog->Size = System::Drawing::Size(716, 157);
			this->textBoxLog->TabIndex = 3;
			// 
			// logTabContextMenu
			// 
			this->logTabContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->contextMenuButtonLogRefresh, 
				this->contextMenuButtonLogOpen, this->toolStripSeparator5, this->contextMenuButtonTrackChanges});
			this->logTabContextMenu->Name = L"logTabContextMenu";
			this->logTabContextMenu->Size = System::Drawing::Size(151, 76);
			// 
			// contextMenuButtonLogRefresh
			// 
			this->contextMenuButtonLogRefresh->Name = L"contextMenuButtonLogRefresh";
			this->contextMenuButtonLogRefresh->Size = System::Drawing::Size(150, 22);
			this->contextMenuButtonLogRefresh->Text = L"Refresh";
			this->contextMenuButtonLogRefresh->Click += gcnew System::EventHandler(this, &FormMain::contextMenuButtonLogRefresh_Click);
			// 
			// contextMenuButtonLogOpen
			// 
			this->contextMenuButtonLogOpen->Name = L"contextMenuButtonLogOpen";
			this->contextMenuButtonLogOpen->Size = System::Drawing::Size(150, 22);
			this->contextMenuButtonLogOpen->Text = L"Open";
			this->contextMenuButtonLogOpen->Click += gcnew System::EventHandler(this, &FormMain::contextMenuButtonLogOpen_Click);
			// 
			// toolStripSeparator5
			// 
			this->toolStripSeparator5->Name = L"toolStripSeparator5";
			this->toolStripSeparator5->Size = System::Drawing::Size(147, 6);
			// 
			// contextMenuButtonTrackChanges
			// 
			this->contextMenuButtonTrackChanges->Checked = true;
			this->contextMenuButtonTrackChanges->CheckOnClick = true;
			this->contextMenuButtonTrackChanges->CheckState = System::Windows::Forms::CheckState::Checked;
			this->contextMenuButtonTrackChanges->Name = L"contextMenuButtonTrackChanges";
			this->contextMenuButtonTrackChanges->Size = System::Drawing::Size(150, 22);
			this->contextMenuButtonTrackChanges->Text = L"Track changes";
			// 
			// tabControlRight
			// 
			this->tabControlRight->Controls->Add(this->tabPageProperties);
			this->tabControlRight->Controls->Add(this->tabPageDebug);
			this->tabControlRight->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControlRight->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->tabControlRight->Location = System::Drawing::Point(0, 0);
			this->tabControlRight->Name = L"tabControlRight";
			this->tabControlRight->SelectedIndex = 0;
			this->tabControlRight->Size = System::Drawing::Size(376, 707);
			this->tabControlRight->TabIndex = 0;
			// 
			// tabPageProperties
			// 
			this->tabPageProperties->Controls->Add(this->groupBoxIOOptions);
			this->tabPageProperties->Controls->Add(this->groupBoxAdvanced);
			this->tabPageProperties->Controls->Add(this->buttonOptionsAdvanced);
			this->tabPageProperties->Controls->Add(this->buttonOptionsDefault);
			this->tabPageProperties->Controls->Add(this->groupBoxLanguage);
			this->tabPageProperties->Controls->Add(this->groupBoxOptions);
			this->tabPageProperties->Location = System::Drawing::Point(4, 22);
			this->tabPageProperties->Name = L"tabPageProperties";
			this->tabPageProperties->Padding = System::Windows::Forms::Padding(3);
			this->tabPageProperties->Size = System::Drawing::Size(368, 681);
			this->tabPageProperties->TabIndex = 0;
			this->tabPageProperties->Text = L"Properties";
			this->tabPageProperties->UseVisualStyleBackColor = true;
			// 
			// groupBoxIOOptions
			// 
			this->groupBoxIOOptions->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBoxIOOptions->Controls->Add(this->textBoxPhOptionsInput);
			this->groupBoxIOOptions->Controls->Add(this->checkBoxOptionRedirectStreams);
			this->groupBoxIOOptions->Location = System::Drawing::Point(15, 489);
			this->groupBoxIOOptions->Name = L"groupBoxIOOptions";
			this->groupBoxIOOptions->Size = System::Drawing::Size(333, 173);
			this->groupBoxIOOptions->TabIndex = 5;
			this->groupBoxIOOptions->TabStop = false;
			this->groupBoxIOOptions->Text = L"Input / output redirection";
			this->groupBoxIOOptions->Visible = false;
			// 
			// textBoxPhOptionsInput
			// 
			this->textBoxPhOptionsInput->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxPhOptionsInput->DefaultValueMode = false;
			this->textBoxPhOptionsInput->Location = System::Drawing::Point(19, 71);
			this->textBoxPhOptionsInput->Multiline = true;
			this->textBoxPhOptionsInput->Name = L"textBoxPhOptionsInput";
			this->textBoxPhOptionsInput->NoWhitespaceMode = false;
			this->textBoxPhOptionsInput->PlaceHolderText = nullptr;
			this->textBoxPhOptionsInput->PlaceHolderTextColor = System::Drawing::SystemColors::ControlDark;
			this->textBoxPhOptionsInput->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxPhOptionsInput->Size = System::Drawing::Size(298, 81);
			this->textBoxPhOptionsInput->TabIndex = 2;
			// 
			// checkBoxOptionRedirectStreams
			// 
			this->checkBoxOptionRedirectStreams->AutoSize = true;
			this->checkBoxOptionRedirectStreams->Location = System::Drawing::Point(19, 34);
			this->checkBoxOptionRedirectStreams->Name = L"checkBoxOptionRedirectStreams";
			this->checkBoxOptionRedirectStreams->Size = System::Drawing::Size(203, 17);
			this->checkBoxOptionRedirectStreams->TabIndex = 1;
			this->checkBoxOptionRedirectStreams->Text = L"Redirect input and output streams";
			this->checkBoxOptionRedirectStreams->UseVisualStyleBackColor = true;
			// 
			// groupBoxAdvanced
			// 
			this->groupBoxAdvanced->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBoxAdvanced->Controls->Add(this->labelOptionsCommandLine);
			this->groupBoxAdvanced->Controls->Add(this->textBoxOptionsCommandLine);
			this->groupBoxAdvanced->Location = System::Drawing::Point(15, 364);
			this->groupBoxAdvanced->Name = L"groupBoxAdvanced";
			this->groupBoxAdvanced->Size = System::Drawing::Size(333, 110);
			this->groupBoxAdvanced->TabIndex = 4;
			this->groupBoxAdvanced->TabStop = false;
			this->groupBoxAdvanced->Text = L"Command line";
			this->groupBoxAdvanced->Visible = false;
			// 
			// labelOptionsCommandLine
			// 
			this->labelOptionsCommandLine->AutoSize = true;
			this->labelOptionsCommandLine->Location = System::Drawing::Point(16, 29);
			this->labelOptionsCommandLine->Name = L"labelOptionsCommandLine";
			this->labelOptionsCommandLine->Size = System::Drawing::Size(260, 13);
			this->labelOptionsCommandLine->TabIndex = 1;
			this->labelOptionsCommandLine->Text = L"Write below additional commands for interpreter";
			// 
			// textBoxOptionsCommandLine
			// 
			this->textBoxOptionsCommandLine->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxOptionsCommandLine->Location = System::Drawing::Point(19, 61);
			this->textBoxOptionsCommandLine->MaxLength = 6144;
			this->textBoxOptionsCommandLine->Name = L"textBoxOptionsCommandLine";
			this->textBoxOptionsCommandLine->Size = System::Drawing::Size(298, 22);
			this->textBoxOptionsCommandLine->TabIndex = 0;
			// 
			// buttonOptionsAdvanced
			// 
			this->buttonOptionsAdvanced->Location = System::Drawing::Point(15, 321);
			this->buttonOptionsAdvanced->Name = L"buttonOptionsAdvanced";
			this->buttonOptionsAdvanced->Size = System::Drawing::Size(164, 23);
			this->buttonOptionsAdvanced->TabIndex = 3;
			this->buttonOptionsAdvanced->Text = L"Show advanced options";
			this->buttonOptionsAdvanced->UseVisualStyleBackColor = true;
			this->buttonOptionsAdvanced->Click += gcnew System::EventHandler(this, &FormMain::buttonOptionsAdvanced_Click);
			// 
			// buttonOptionsDefault
			// 
			this->buttonOptionsDefault->Location = System::Drawing::Point(185, 321);
			this->buttonOptionsDefault->Name = L"buttonOptionsDefault";
			this->buttonOptionsDefault->Size = System::Drawing::Size(122, 23);
			this->buttonOptionsDefault->TabIndex = 2;
			this->buttonOptionsDefault->Text = L"Default";
			this->buttonOptionsDefault->UseVisualStyleBackColor = true;
			this->buttonOptionsDefault->Click += gcnew System::EventHandler(this, &FormMain::buttonOptionsDefault_Click);
			// 
			// groupBoxLanguage
			// 
			this->groupBoxLanguage->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBoxLanguage->Controls->Add(this->comboBoxOptionsLanguage);
			this->groupBoxLanguage->Location = System::Drawing::Point(15, 19);
			this->groupBoxLanguage->Name = L"groupBoxLanguage";
			this->groupBoxLanguage->Size = System::Drawing::Size(333, 77);
			this->groupBoxLanguage->TabIndex = 1;
			this->groupBoxLanguage->TabStop = false;
			this->groupBoxLanguage->Text = L"Language";
			// 
			// comboBoxOptionsLanguage
			// 
			this->comboBoxOptionsLanguage->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->comboBoxOptionsLanguage->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxOptionsLanguage->FormattingEnabled = true;
			this->comboBoxOptionsLanguage->Location = System::Drawing::Point(19, 31);
			this->comboBoxOptionsLanguage->Name = L"comboBoxOptionsLanguage";
			this->comboBoxOptionsLanguage->Size = System::Drawing::Size(299, 21);
			this->comboBoxOptionsLanguage->TabIndex = 0;
			this->comboBoxOptionsLanguage->SelectionChangeCommitted += gcnew System::EventHandler(this, &FormMain::comboBoxOptionsLanguage_SelectionChangeCommitted);
			// 
			// groupBoxOptions
			// 
			this->groupBoxOptions->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBoxOptions->Controls->Add(this->comboBoxOptionsEof);
			this->groupBoxOptions->Controls->Add(this->comboBoxOptionsMemoryBehavior);
			this->groupBoxOptions->Controls->Add(this->comboBoxOptionsCellSize);
			this->groupBoxOptions->Controls->Add(this->labelOptionsEof);
			this->groupBoxOptions->Controls->Add(this->labelOptionsMemoryBehavior);
			this->groupBoxOptions->Controls->Add(this->labelOptionsCellSize);
			this->groupBoxOptions->Controls->Add(this->textBoxOptionsMemorySize);
			this->groupBoxOptions->Controls->Add(this->labelOptionsMemorySize);
			this->groupBoxOptions->Location = System::Drawing::Point(15, 112);
			this->groupBoxOptions->Name = L"groupBoxOptions";
			this->groupBoxOptions->Size = System::Drawing::Size(333, 188);
			this->groupBoxOptions->TabIndex = 0;
			this->groupBoxOptions->TabStop = false;
			this->groupBoxOptions->Text = L"Options";
			// 
			// comboBoxOptionsEof
			// 
			this->comboBoxOptionsEof->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->comboBoxOptionsEof->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxOptionsEof->FormattingEnabled = true;
			this->comboBoxOptionsEof->Location = System::Drawing::Point(122, 140);
			this->comboBoxOptionsEof->Name = L"comboBoxOptionsEof";
			this->comboBoxOptionsEof->Size = System::Drawing::Size(196, 21);
			this->comboBoxOptionsEof->TabIndex = 7;
			// 
			// comboBoxOptionsMemoryBehavior
			// 
			this->comboBoxOptionsMemoryBehavior->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->comboBoxOptionsMemoryBehavior->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxOptionsMemoryBehavior->FormattingEnabled = true;
			this->comboBoxOptionsMemoryBehavior->Location = System::Drawing::Point(122, 105);
			this->comboBoxOptionsMemoryBehavior->Name = L"comboBoxOptionsMemoryBehavior";
			this->comboBoxOptionsMemoryBehavior->Size = System::Drawing::Size(196, 21);
			this->comboBoxOptionsMemoryBehavior->TabIndex = 6;
			// 
			// comboBoxOptionsCellSize
			// 
			this->comboBoxOptionsCellSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->comboBoxOptionsCellSize->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxOptionsCellSize->FormattingEnabled = true;
			this->comboBoxOptionsCellSize->Location = System::Drawing::Point(122, 70);
			this->comboBoxOptionsCellSize->Name = L"comboBoxOptionsCellSize";
			this->comboBoxOptionsCellSize->Size = System::Drawing::Size(196, 21);
			this->comboBoxOptionsCellSize->TabIndex = 5;
			// 
			// labelOptionsEof
			// 
			this->labelOptionsEof->AutoSize = true;
			this->labelOptionsEof->Location = System::Drawing::Point(16, 143);
			this->labelOptionsEof->Name = L"labelOptionsEof";
			this->labelOptionsEof->Size = System::Drawing::Size(79, 13);
			this->labelOptionsEof->TabIndex = 4;
			this->labelOptionsEof->Text = L"EOF behavior:";
			// 
			// labelOptionsMemoryBehavior
			// 
			this->labelOptionsMemoryBehavior->AutoSize = true;
			this->labelOptionsMemoryBehavior->Location = System::Drawing::Point(16, 108);
			this->labelOptionsMemoryBehavior->Name = L"labelOptionsMemoryBehavior";
			this->labelOptionsMemoryBehavior->Size = System::Drawing::Size(99, 13);
			this->labelOptionsMemoryBehavior->TabIndex = 3;
			this->labelOptionsMemoryBehavior->Text = L"Memory behavior:";
			// 
			// labelOptionsCellSize
			// 
			this->labelOptionsCellSize->AutoSize = true;
			this->labelOptionsCellSize->Location = System::Drawing::Point(16, 73);
			this->labelOptionsCellSize->Name = L"labelOptionsCellSize";
			this->labelOptionsCellSize->Size = System::Drawing::Size(51, 13);
			this->labelOptionsCellSize->TabIndex = 2;
			this->labelOptionsCellSize->Text = L"Cell size:";
			// 
			// textBoxOptionsMemorySize
			// 
			this->textBoxOptionsMemorySize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxOptionsMemorySize->Location = System::Drawing::Point(122, 35);
			this->textBoxOptionsMemorySize->MaxLength = 10;
			this->textBoxOptionsMemorySize->Name = L"textBoxOptionsMemorySize";
			this->textBoxOptionsMemorySize->Size = System::Drawing::Size(196, 22);
			this->textBoxOptionsMemorySize->TabIndex = 1;
			this->textBoxOptionsMemorySize->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormMain::textBoxOptionsMemorySize_KeyPress);
			// 
			// labelOptionsMemorySize
			// 
			this->labelOptionsMemorySize->AutoSize = true;
			this->labelOptionsMemorySize->Location = System::Drawing::Point(16, 38);
			this->labelOptionsMemorySize->Name = L"labelOptionsMemorySize";
			this->labelOptionsMemorySize->Size = System::Drawing::Size(73, 13);
			this->labelOptionsMemorySize->TabIndex = 0;
			this->labelOptionsMemorySize->Text = L"Memory size:";
			// 
			// tabPageDebug
			// 
			this->tabPageDebug->Controls->Add(this->panelDebug);
			this->tabPageDebug->Controls->Add(this->splitContainerDebugTools);
			this->tabPageDebug->Location = System::Drawing::Point(4, 22);
			this->tabPageDebug->Name = L"tabPageDebug";
			this->tabPageDebug->Padding = System::Windows::Forms::Padding(3);
			this->tabPageDebug->Size = System::Drawing::Size(368, 681);
			this->tabPageDebug->TabIndex = 1;
			this->tabPageDebug->Text = L"Debug";
			this->tabPageDebug->UseVisualStyleBackColor = true;
			// 
			// panelDebug
			// 
			this->panelDebug->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panelDebug->Controls->Add(this->buttonDebugStop);
			this->panelDebug->Controls->Add(this->labelDebugStatus);
			this->panelDebug->Controls->Add(this->buttonDebugPlaceCursor);
			this->panelDebug->Controls->Add(this->buttonDebugStep);
			this->panelDebug->Controls->Add(this->buttonDebugStepOver);
			this->panelDebug->Controls->Add(this->buttonDebugRunToCursor);
			this->panelDebug->Location = System::Drawing::Point(15, 19);
			this->panelDebug->Name = L"panelDebug";
			this->panelDebug->Size = System::Drawing::Size(338, 100);
			this->panelDebug->TabIndex = 7;
			// 
			// buttonDebugStop
			// 
			this->buttonDebugStop->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"buttonDebugStop.BackgroundImage")));
			this->buttonDebugStop->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->buttonDebugStop->Location = System::Drawing::Point(226, 17);
			this->buttonDebugStop->Name = L"buttonDebugStop";
			this->buttonDebugStop->Size = System::Drawing::Size(38, 32);
			this->buttonDebugStop->TabIndex = 8;
			this->buttonDebugStop->UseVisualStyleBackColor = true;
			this->buttonDebugStop->Click += gcnew System::EventHandler(this, &FormMain::stopMainMenuItem_Click);
			// 
			// labelDebugStatus
			// 
			this->labelDebugStatus->AutoSize = true;
			this->labelDebugStatus->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->labelDebugStatus->Location = System::Drawing::Point(19, 64);
			this->labelDebugStatus->Name = L"labelDebugStatus";
			this->labelDebugStatus->Size = System::Drawing::Size(164, 15);
			this->labelDebugStatus->TabIndex = 7;
			this->labelDebugStatus->Text = L"Click Step to start debugging";
			// 
			// buttonDebugPlaceCursor
			// 
			this->buttonDebugPlaceCursor->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"buttonDebugPlaceCursor.BackgroundImage")));
			this->buttonDebugPlaceCursor->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->buttonDebugPlaceCursor->Location = System::Drawing::Point(270, 17);
			this->buttonDebugPlaceCursor->Name = L"buttonDebugPlaceCursor";
			this->buttonDebugPlaceCursor->Size = System::Drawing::Size(38, 32);
			this->buttonDebugPlaceCursor->TabIndex = 6;
			this->buttonDebugPlaceCursor->UseVisualStyleBackColor = true;
			this->buttonDebugPlaceCursor->Click += gcnew System::EventHandler(this, &FormMain::buttonDebugPlaceCursor_Click);
			// 
			// buttonDebugStep
			// 
			this->buttonDebugStep->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"buttonDebugStep.BackgroundImage")));
			this->buttonDebugStep->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->buttonDebugStep->Location = System::Drawing::Point(22, 17);
			this->buttonDebugStep->Name = L"buttonDebugStep";
			this->buttonDebugStep->Size = System::Drawing::Size(38, 32);
			this->buttonDebugStep->TabIndex = 2;
			this->buttonDebugStep->UseVisualStyleBackColor = true;
			this->buttonDebugStep->Click += gcnew System::EventHandler(this, &FormMain::buttonDebugStep_Click);
			// 
			// buttonDebugStepOver
			// 
			this->buttonDebugStepOver->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"buttonDebugStepOver.BackgroundImage")));
			this->buttonDebugStepOver->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->buttonDebugStepOver->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->buttonDebugStepOver->Location = System::Drawing::Point(66, 17);
			this->buttonDebugStepOver->Name = L"buttonDebugStepOver";
			this->buttonDebugStepOver->Size = System::Drawing::Size(38, 32);
			this->buttonDebugStepOver->TabIndex = 3;
			this->buttonDebugStepOver->UseVisualStyleBackColor = true;
			this->buttonDebugStepOver->Click += gcnew System::EventHandler(this, &FormMain::buttonDebugStepOver_Click);
			// 
			// buttonDebugRunToCursor
			// 
			this->buttonDebugRunToCursor->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"buttonDebugRunToCursor.BackgroundImage")));
			this->buttonDebugRunToCursor->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->buttonDebugRunToCursor->Location = System::Drawing::Point(110, 17);
			this->buttonDebugRunToCursor->Name = L"buttonDebugRunToCursor";
			this->buttonDebugRunToCursor->Size = System::Drawing::Size(38, 32);
			this->buttonDebugRunToCursor->TabIndex = 4;
			this->buttonDebugRunToCursor->UseVisualStyleBackColor = true;
			this->buttonDebugRunToCursor->Click += gcnew System::EventHandler(this, &FormMain::buttonDebugRunToCursor_Click);
			// 
			// splitContainerDebugTools
			// 
			this->splitContainerDebugTools->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->splitContainerDebugTools->Location = System::Drawing::Point(15, 125);
			this->splitContainerDebugTools->Name = L"splitContainerDebugTools";
			this->splitContainerDebugTools->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainerDebugTools.Panel1
			// 
			this->splitContainerDebugTools->Panel1->Controls->Add(this->treeViewDebugThreadList);
			// 
			// splitContainerDebugTools.Panel2
			// 
			this->splitContainerDebugTools->Panel2->Controls->Add(this->comboBoxThreadMemSelect);
			this->splitContainerDebugTools->Panel2->Controls->Add(this->dataGridViewDebugMemory);
			this->splitContainerDebugTools->Size = System::Drawing::Size(338, 538);
			this->splitContainerDebugTools->SplitterDistance = 186;
			this->splitContainerDebugTools->SplitterWidth = 7;
			this->splitContainerDebugTools->TabIndex = 6;
			// 
			// treeViewDebugThreadList
			// 
			this->treeViewDebugThreadList->ContextMenuStrip = this->threadContextMenu;
			this->treeViewDebugThreadList->Dock = System::Windows::Forms::DockStyle::Fill;
			this->treeViewDebugThreadList->Location = System::Drawing::Point(0, 0);
			this->treeViewDebugThreadList->Name = L"treeViewDebugThreadList";
			this->treeViewDebugThreadList->Size = System::Drawing::Size(338, 186);
			this->treeViewDebugThreadList->TabIndex = 6;
			this->treeViewDebugThreadList->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &FormMain::treeViewDebugThreadList_AfterSelect);
			this->treeViewDebugThreadList->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &FormMain::treeViewDebugThreadList_MouseUp);
			// 
			// threadContextMenu
			// 
			this->threadContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->contextMenuButtonTrackThread, 
				this->toolStripSeparator7, this->contextMenuButtonExpandAll, this->contextMenuButtonCollapseAll, this->contextMenuButtonCollapseTree});
			this->threadContextMenu->Name = L"threadContextMenu";
			this->threadContextMenu->Size = System::Drawing::Size(163, 98);
			// 
			// contextMenuButtonTrackThread
			// 
			this->contextMenuButtonTrackThread->CheckOnClick = true;
			this->contextMenuButtonTrackThread->Name = L"contextMenuButtonTrackThread";
			this->contextMenuButtonTrackThread->Size = System::Drawing::Size(162, 22);
			this->contextMenuButtonTrackThread->Text = L"Trace this thread";
			this->contextMenuButtonTrackThread->Visible = false;
			this->contextMenuButtonTrackThread->Click += gcnew System::EventHandler(this, &FormMain::contextMenuButtonTrackThread_Click);
			// 
			// toolStripSeparator7
			// 
			this->toolStripSeparator7->Name = L"toolStripSeparator7";
			this->toolStripSeparator7->Size = System::Drawing::Size(159, 6);
			this->toolStripSeparator7->Visible = false;
			// 
			// contextMenuButtonExpandAll
			// 
			this->contextMenuButtonExpandAll->Name = L"contextMenuButtonExpandAll";
			this->contextMenuButtonExpandAll->Size = System::Drawing::Size(162, 22);
			this->contextMenuButtonExpandAll->Text = L"Expand tree";
			this->contextMenuButtonExpandAll->Click += gcnew System::EventHandler(this, &FormMain::contextMenuButtonExpandAll_Click);
			// 
			// contextMenuButtonCollapseAll
			// 
			this->contextMenuButtonCollapseAll->Name = L"contextMenuButtonCollapseAll";
			this->contextMenuButtonCollapseAll->Size = System::Drawing::Size(162, 22);
			this->contextMenuButtonCollapseAll->Text = L"Collapse tree";
			this->contextMenuButtonCollapseAll->Click += gcnew System::EventHandler(this, &FormMain::contextMenuButtonExpandAll_Click);
			// 
			// contextMenuButtonCollapseTree
			// 
			this->contextMenuButtonCollapseTree->Name = L"contextMenuButtonCollapseTree";
			this->contextMenuButtonCollapseTree->Size = System::Drawing::Size(162, 22);
			this->contextMenuButtonCollapseTree->Text = L"Collapse threads";
			this->contextMenuButtonCollapseTree->Click += gcnew System::EventHandler(this, &FormMain::contextMenuButtonExpandAll_Click);
			// 
			// comboBoxThreadMemSelect
			// 
			this->comboBoxThreadMemSelect->Dock = System::Windows::Forms::DockStyle::Top;
			this->comboBoxThreadMemSelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxThreadMemSelect->FormattingEnabled = true;
			this->comboBoxThreadMemSelect->Location = System::Drawing::Point(0, 0);
			this->comboBoxThreadMemSelect->Name = L"comboBoxThreadMemSelect";
			this->comboBoxThreadMemSelect->Size = System::Drawing::Size(338, 21);
			this->comboBoxThreadMemSelect->TabIndex = 3;
			this->comboBoxThreadMemSelect->SelectionChangeCommitted += gcnew System::EventHandler(this, &FormMain::comboBoxThreadMemSelect_SelectionChangeCommitted);
			// 
			// dataGridViewDebugMemory
			// 
			this->dataGridViewDebugMemory->AllowUserToAddRows = false;
			this->dataGridViewDebugMemory->AllowUserToDeleteRows = false;
			this->dataGridViewDebugMemory->AllowUserToResizeColumns = false;
			this->dataGridViewDebugMemory->AllowUserToResizeRows = false;
			this->dataGridViewDebugMemory->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridViewDebugMemory->BackgroundColor = System::Drawing::SystemColors::Control;
			this->dataGridViewDebugMemory->ClipboardCopyMode = System::Windows::Forms::DataGridViewClipboardCopyMode::Disable;
			this->dataGridViewDebugMemory->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			this->dataGridViewDebugMemory->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
			this->dataGridViewDebugMemory->ContextMenuStrip = this->debugMemViewContextMenu;
			this->dataGridViewDebugMemory->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
			this->dataGridViewDebugMemory->Location = System::Drawing::Point(0, 27);
			this->dataGridViewDebugMemory->MultiSelect = false;
			this->dataGridViewDebugMemory->Name = L"dataGridViewDebugMemory";
			this->dataGridViewDebugMemory->ReadOnly = true;
			this->dataGridViewDebugMemory->RowHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			this->dataGridViewDebugMemory->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			this->dataGridViewDebugMemory->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->dataGridViewDebugMemory->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
			this->dataGridViewDebugMemory->ShowCellErrors = false;
			this->dataGridViewDebugMemory->ShowCellToolTips = false;
			this->dataGridViewDebugMemory->ShowEditingIcon = false;
			this->dataGridViewDebugMemory->ShowRowErrors = false;
			this->dataGridViewDebugMemory->Size = System::Drawing::Size(338, 267);
			this->dataGridViewDebugMemory->TabIndex = 2;
			this->dataGridViewDebugMemory->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &FormMain::dataGridViewDebugMemory_Scroll);
			// 
			// debugMemViewContextMenu
			// 
			this->debugMemViewContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->contextMenuButtonScrollToMemPos, 
				this->contextMenuButtonScrollToLNCell, this->toolStripSeparator6, this->contextMenuButtonMemoryRepresentation, this->contextMenuButtonColumnNumber});
			this->debugMemViewContextMenu->Name = L"debugMemViewContextMenu";
			this->debugMemViewContextMenu->Size = System::Drawing::Size(211, 98);
			// 
			// contextMenuButtonScrollToMemPos
			// 
			this->contextMenuButtonScrollToMemPos->Name = L"contextMenuButtonScrollToMemPos";
			this->contextMenuButtonScrollToMemPos->Size = System::Drawing::Size(210, 22);
			this->contextMenuButtonScrollToMemPos->Text = L"Scroll to pointer position";
			this->contextMenuButtonScrollToMemPos->Click += gcnew System::EventHandler(this, &FormMain::contextMenuButtonScrollToMemPos_Click);
			// 
			// contextMenuButtonScrollToLNCell
			// 
			this->contextMenuButtonScrollToLNCell->Name = L"contextMenuButtonScrollToLNCell";
			this->contextMenuButtonScrollToLNCell->Size = System::Drawing::Size(210, 22);
			this->contextMenuButtonScrollToLNCell->Text = L"Scroll to last non-zero cell";
			this->contextMenuButtonScrollToLNCell->Click += gcnew System::EventHandler(this, &FormMain::contextMenuButtonScrollToLNCell_Click);
			// 
			// toolStripSeparator6
			// 
			this->toolStripSeparator6->Name = L"toolStripSeparator6";
			this->toolStripSeparator6->Size = System::Drawing::Size(207, 6);
			// 
			// contextMenuButtonMemoryRepresentation
			// 
			this->contextMenuButtonMemoryRepresentation->Name = L"contextMenuButtonMemoryRepresentation";
			this->contextMenuButtonMemoryRepresentation->Size = System::Drawing::Size(210, 22);
			this->contextMenuButtonMemoryRepresentation->Text = L"Memory represenation";
			// 
			// contextMenuButtonColumnNumber
			// 
			this->contextMenuButtonColumnNumber->Name = L"contextMenuButtonColumnNumber";
			this->contextMenuButtonColumnNumber->Size = System::Drawing::Size(210, 22);
			this->contextMenuButtonColumnNumber->Text = L"Columns";
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1110, 778);
			this->Controls->Add(this->splitContainerMain);
			this->Controls->Add(this->toolBar);
			this->Controls->Add(this->statusBar);
			this->Controls->Add(this->mainMenu);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->MainMenuStrip = this->mainMenu;
			this->Name = L"FormMain";
			this->Text = L"BrainthreadIDE";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
			this->editorContextMenu->ResumeLayout(false);
			this->statusBar->ResumeLayout(false);
			this->statusBar->PerformLayout();
			this->mainMenu->ResumeLayout(false);
			this->mainMenu->PerformLayout();
			this->toolBar->ResumeLayout(false);
			this->toolBar->PerformLayout();
			this->splitContainerMain->Panel1->ResumeLayout(false);
			this->splitContainerMain->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerMain))->EndInit();
			this->splitContainerMain->ResumeLayout(false);
			this->splitContainerEditor->Panel1->ResumeLayout(false);
			this->splitContainerEditor->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerEditor))->EndInit();
			this->splitContainerEditor->ResumeLayout(false);
			this->tabControlOutput->ResumeLayout(false);
			this->tabPageErrorList->ResumeLayout(false);
			this->tabPageOutputList->ResumeLayout(false);
			this->tabPageOutputList->PerformLayout();
			this->tabPageLog->ResumeLayout(false);
			this->tabPageLog->PerformLayout();
			this->logTabContextMenu->ResumeLayout(false);
			this->tabControlRight->ResumeLayout(false);
			this->tabPageProperties->ResumeLayout(false);
			this->groupBoxIOOptions->ResumeLayout(false);
			this->groupBoxIOOptions->PerformLayout();
			this->groupBoxAdvanced->ResumeLayout(false);
			this->groupBoxAdvanced->PerformLayout();
			this->groupBoxLanguage->ResumeLayout(false);
			this->groupBoxOptions->ResumeLayout(false);
			this->groupBoxOptions->PerformLayout();
			this->tabPageDebug->ResumeLayout(false);
			this->panelDebug->ResumeLayout(false);
			this->panelDebug->PerformLayout();
			this->splitContainerDebugTools->Panel1->ResumeLayout(false);
			this->splitContainerDebugTools->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainerDebugTools))->EndInit();
			this->splitContainerDebugTools->ResumeLayout(false);
			this->threadContextMenu->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewDebugMemory))->EndInit();
			this->debugMemViewContextMenu->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
public: void editorRichTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			     
#ifdef _DEBUG
				 if(WorkContextBroker::Instance->isNotinitialized())
					return;
#endif

			     WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
				 currentWorkContext->syntaxHighLighter->HighLightLastChar(currentWorkContext->editorTextBox->richTextBox);	 
				
				 mainForm_UpdateUI();
		}
public: void editorRichTextBox_SelectionChanged(System::Object^  sender, System::EventArgs^  e) {
			     
				 mainForm_SetStatusBarInfo(WorkContextBroker::Instance->GetCurrentContext());
		}
public: void mainForm_ReadLog(String ^ path) {
				 FileContext ^ logFileContext = gcnew FileContext(path);

				 if(logFileContext->Open()) {
					 if(this->contextMenuButtonTrackChanges->Checked)
					 {
						 if(String::Compare(logFileContext->Content, this->textBoxLog->Text))
							 this->tabPageLog->Text = "Log *";
						 else
							 this->tabPageLog->Text = "Log";
					 }

					 this->textBoxLog->Text = logFileContext->Content;
				 }
				 else {
					 this->textBoxLog->Text = String::Format("Cannot load file '{0}'", logFileContext->FilePath);
				 }
		}
public: void mainForm_ReadLog(void) {
			     
			     if(GlobalOptions::Instance->ReadLogFlag == false)
					 return;

				 mainForm_ReadLog(GlobalOptions::Instance->LogPath);
		}
private: void mainForm_ChangeContext(TabPage ^ selectedPage) {
				 
			     WorkContext ^ newWorkContext = newWorkContext = WorkContextBroker::Instance->GetCurrentContext();
				 if(newWorkContext)
				 {
					 newWorkContext->settings->Load();

					 //output
					 newWorkContext->outputLister->Refresh();
					 //if message list is not empty, select that tab
					 if(this->listViewOutputErrors->Items->Count == 0) {
						 this->tabControlOutput->SelectedTab = this->tabPageOutputList;
					 }

					 //ui
					 mainForm_UpdateUI();
					 mainForm_SetStatusBarInfo(newWorkContext);
				 }
		}
private: void mainForm_SetStatusBarInfo(WorkContext ^ workContext) {
				 //status bar carret info update

				 if(workContext) //work context info update
				 {
					 int cur_pos, text_len;
					 String ^ selection_info;
					 CodeContextConverter ^ ccc = gcnew CodeContextConverter(workContext);
					 
					 if(workContext->editorTextBox->richTextBox->SelectionLength) {
						 selection_info = String::Format("Selected {0}", workContext->editorTextBox->richTextBox->SelectionLength);
					 }

					 cur_pos = workContext->editorTextBox->richTextBox->SelectionStart;
					 text_len = workContext->editorTextBox->richTextBox->Text->Length;

					 //setting captions
					 statusStripStatusLabel1->Text = workContext->fileContext->Title;
					 if(workContext->fileContext->IsModified) {
						 statusStripStatusLabel1->Text += " - Modified";
					 }

					 if(text_len > 0) {
						 statusStripStatusLabel3->Text = String::Format("Line {0}   Col {1}   Char {2}  {3}", 
									workContext->editorTextBox->richTextBox->GetLineFromCharIndex(cur_pos),
									cur_pos - workContext->editorTextBox->richTextBox->GetFirstCharIndexOfCurrentLine(),
									cur_pos,
									selection_info);

						 statusStripStatusLabel4->Text = String::Format("Instruction {0}", 
									ccc->ToInstruction(cur_pos));
					 }

					 statusStripStatusLabel5->Text = FileSizeConverter::GetSize(text_len);
					 statusStripStatusLabel6->Text = LanguageName::GetLanguageName(workContext->settings->GetLanguage());
				 }
				 else 
					 statusStripStatusLabel1->Text = "none";

				 //processes info update
				 if(interpreterProcess) {
					 statusStripStatusLabel2->Text = interpreterProcess->GetStatusLabel();
					 
					 if(workContext != interpreterProcess->ProcessWorkContext) {
							statusStripStatusLabel2->Text = String::Format("{0} ({1})", 
														statusStripStatusLabel2->Text,
														interpreterProcess->ProcessWorkContext->fileContext->Title);
					 }

				 }
				 else 
					 statusStripStatusLabel2->Text = "Ready";

				 //Form Caption
				 this->Text = String::Format("{0} {1}- BrainthreadIDE", 
								workContext->fileContext->Title,
								interpreterProcess ? statusStripStatusLabel2->Text + " " : "");

		}
private: void mainForm_RunTask() {
				//work todo after spawn a process
				WorkContext ^ workContext;
			    PragmaResolver ^ pragmaResolver;

				if(interpreterProcess)
				{
					workContext = interpreterProcess->ProcessWorkContext;
					pragmaResolver = gcnew PragmaResolver(workContext->editorTextBox->richTextBox->Text, 
														  GlobalOptions::Instance->Plugins);

					workContext->outputLister->Purge();
					
					if(pragmaResolver->HasPragmas() && workContext->editorTextBox->richTextBox->SelectionLength == 0) {
						pragmaResolver->Resolve(workContext, workContext->settings->GetLanguage());
					}

					workContext->settings->Save();

					interpreterProcess->OnStart += gcnew System::EventHandler(this, &FormMain::interpreterProcess_Start); 
					interpreterProcess->OnComplete += gcnew System::EventHandler(this, &FormMain::interpreterProcess_Complete); 

					interpreterProcess->Launch();
				}
		 }
private: void mainForm_RunDebugTask() {
			 //work todo after step, step over, run to cursor etc
				DebugCodeProcess ^ debugProcess = cli::safe_cast<DebugCodeProcess^ >(this->interpreterProcess);
				int n_frow, frow = dataGridViewDebugMemory->FirstDisplayedScrollingRowIndex; //save old first displayed row index

				debugProcess->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory);
				debugForm_MemoryScrollToPosition( debugProcess->DebuggerInstance->MemoryPosition );

				//smooth pointer moving ->first visible row modification
				n_frow = dataGridViewDebugMemory->FirstDisplayedScrollingRowIndex;
				if(frow >= 0 && frow < n_frow && (n_frow - frow) < dataGridViewDebugMemory->DisplayedRowCount(false)) {
					dataGridViewDebugMemory->FirstDisplayedScrollingRowIndex = frow;
				}

				debugProcess->DebuggerInstance->LoadThreadTree(treeViewDebugThreadList);
				debugProcess->DebuggerInstance->LoadThreadList(comboBoxThreadMemSelect);
				debugProcess->DebuggerInstance->LoadStatus(labelDebugStatus);

				if(WorkContextBroker::Instance->GetCurrentContext() != debugProcess->ProcessWorkContext)
				{
					//select debugging page if necessary
					TabPage ^ debugPage = WorkContextBroker::Instance->GetPage(debugProcess->ProcessWorkContext);
 					if(debugPage) {
						this->tabControlPages->SelectedTab = debugPage;
						this->tabControlRight->SelectedTab = this->tabPageDebug;
						mainForm_ChangeContext(debugPage);
					}
				}
		 }
private: bool mainForm_IsDebugTask() {
			//check for type of process is debug process
			    return (interpreterProcess && interpreterProcess->GetType() == DebugCodeProcess::typeid);
		 }

private: void mainForm_UpdateUI() {
			//sets enable state for buttons	 
			     bool genuineInterpreter = false;
				 bool noInterpreter = false;
				 bool noDebuggingPage = false;

				 WorkContext ^ workContext = WorkContextBroker::Instance->GetCurrentContext();
				 
				 //enable if workcontext is valid and NOT while debugging
				 if(workContext && (false == mainForm_IsDebugTask() || workContext != interpreterProcess->ProcessWorkContext )) 
				 {
					 //undos
					 this->editorUndoMenuItem->Enabled = workContext->editorTextBox->CanUndo();
					 this->editorRedoMenuItem->Enabled = workContext->editorTextBox->CanRedo();
					 this->toolBarButtonUndo->Enabled = workContext->editorTextBox->CanUndo();
					 this->toolBarButtonRedo->Enabled = workContext->editorTextBox->CanRedo();
					 
					 //proj
					 this->editorRestoreMenuItem->Enabled = (workContext->fileContext->GetType() == ProjectFileContext::typeid);

					 //clipboard
					 this->editorCutMenuItem->Enabled = true;	
					 this->editorCopyMenuItem->Enabled = true;
					 this->editorPasteMenuItem->Enabled = Clipboard::ContainsText();
					 //this->toolBarButtonCopy->Enabled = true;
					 this->toolBarButtonCut->Enabled = true;
					 this->toolBarButtonPaste->Enabled = Clipboard::ContainsText();	
					 this->editorInsertMenuItem->Enabled = true;
				 }
				 else {
					 this->editorUndoMenuItem->Enabled = false;
					 this->editorRedoMenuItem->Enabled = false;
					 this->toolBarButtonUndo->Enabled = false;
					 this->toolBarButtonRedo->Enabled = false;
					 this->editorRestoreMenuItem->Enabled = false;
					 this->editorCutMenuItem->Enabled = false;	
					 this->editorCopyMenuItem->Enabled = false;
					 this->editorPasteMenuItem->Enabled = false;
					 //this->toolBarButtonCopy->Enabled = false;
					 this->toolBarButtonCut->Enabled = false;
					 this->toolBarButtonPaste->Enabled = false;	
					 this->editorInsertMenuItem->Enabled = false;	
				 }

				 //lock debug, parse and analyse for custom interpreters
				 genuineInterpreter = (workContext && false == GlobalOptions::Instance->CustomInterpreterFlag[ workContext->settings->GetLanguage() ]);
				 noInterpreter = (workContext == nullptr || String::IsNullOrEmpty( GlobalOptions::Instance->InterpreterPath[ workContext->settings->GetLanguage() ]));
				 noDebuggingPage = genuineInterpreter && mainForm_IsDebugTask() && 
								   interpreterProcess->Working() && workContext != interpreterProcess->ProcessWorkContext;

				 this->tabPageDebug->Enabled = noDebuggingPage ? false : genuineInterpreter;

				 if(interpreterProcess == nullptr || interpreterProcess->Working() == false) //only when no process is running
				 {
					 this->parseMainMenuItem->Enabled = genuineInterpreter;
					 this->analyseMainMenuItem->Enabled = genuineInterpreter;
					 this->debugMainMenuItem->Enabled = genuineInterpreter;
					 this->runMainMenuItem->Enabled = !noInterpreter;
					 this->runSelectionMainMenuItem->Enabled = !noInterpreter;
					 this->runSeparateMainMenuItem->Enabled = !noInterpreter;
					 this->toolBarButtonRunCode->Enabled = !noInterpreter;
					 this->groupBoxOptions->Enabled = genuineInterpreter;
				
					 //debugger
					 this->toolBarButtonDebugStep->Enabled = genuineInterpreter;
					 this->toolBarButtonStepOver->Enabled = genuineInterpreter;
					 this->toolBarButtonRunToCursor->Enabled = genuineInterpreter;
				 }

				 //debug panel label
				 if(noDebuggingPage)
					 this->labelDebugStatus->Text = cDebugLabelBusy;
				 else if(mainForm_IsDebugTask() && this->labelDebugStatus->Tag)
					 this->labelDebugStatus->Text = dynamic_cast<String ^>(this->labelDebugStatus->Tag);
				 else if(genuineInterpreter == false)
					 this->labelDebugStatus->Text = cDebugLabelInfoDisabled;	
				 else
					 this->labelDebugStatus->Text = cDebugLabelInfo;

		}
public: void mainForm_ToggleInterface(InterpreterProcess ^sender, bool normal_state) {
				//sets ui state while process run	 
				bool debugger_running = sender && (sender->GetType() == DebugCodeProcess::typeid) && normal_state == false;
			
				this->parseMainMenuItem->Enabled = normal_state;
				this->analyseMainMenuItem->Enabled = normal_state;
				this->debugMainMenuItem->Enabled = normal_state;
				this->runMainMenuItem->Enabled = normal_state;
				this->runSelectionMainMenuItem->Enabled = normal_state;
				this->runSeparateMainMenuItem->Enabled = normal_state;
				this->toolBarButtonRunCode->Enabled = normal_state;

				this->stopMainMenuItem->Enabled = !normal_state;
				this->buttonDebugStop->Enabled = !normal_state;

				this->detachMainMenuItem->Visible = !normal_state;
				
				//debugger
				this->toolBarButtonDebugStep->Enabled = debugger_running || normal_state;
				this->toolBarButtonStepOver->Enabled = debugger_running;
				this->toolBarButtonRunToCursor->Enabled = debugger_running;
				this->buttonDebugStep->Enabled = debugger_running || normal_state;
				this->buttonDebugStepOver->Enabled = debugger_running;
				this->buttonDebugRunToCursor->Enabled = debugger_running;
				this->buttonDebugPlaceCursor->Enabled = debugger_running;
				
				if(debugger_running)
					this->dataGridViewDebugMemory->ContextMenuStrip  = this->debugMemViewContextMenu;
				else
					this->dataGridViewDebugMemory->ContextMenuStrip = nullptr;

				if(normal_state)
					this->toolBarButtonDebugStep->ToolTipText = cTooltipDebugCodeCaption;
				else
					this->toolBarButtonDebugStep->ToolTipText = cTooltipDebugStepCaption;

		}
private: TabPage ^ GetTabPageByCursorLocation(Point pt) {
			//for drag n drop - fins tab page over cursor
			for(int i = 0; i < this->tabControlPages->TabPages->Count; ++i) {
				if(this->tabControlPages->GetTabRect(i).Contains(pt)) {
					return this->tabControlPages->TabPages[i];
				}
			}
    
			return nullptr;
		}
private: void debugForm_MemoryScrollToPosition(int position) {
			 //scroll to memory pos
			 DebugCodeProcess ^ debugProcess = cli::safe_cast<DebugCodeProcess^ >(interpreterProcess);

			 dataGridViewDebugMemory->ClearSelection();

			 if(debugProcess && dataGridViewDebugMemory->Columns->Count && position >= 0)
			 {
			     //memory not loaded 
				 if(debugProcess->DebuggerInstance->MemoryForImage < position && debugProcess->Working()) 
				 {
					 if(debugProcess->DebuggerInstance->CurrentThreadId != //not current thread
						 Convert::ToInt32(comboBoxThreadMemSelect->SelectedValue))
					 {
						 MessageBox::Show(String::Format("Position {0} is not reachable.",position),"Scroll error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
						 return;
					 }

					 debugProcess->DebuggerInstance->MemoryForImage = position;

					 debugProcess->DebuggerInstance->LoadMemoryByThread();
					 debugProcess->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory);

					 if(debugProcess->DebuggerInstance->MemoryForImage < position) //failed resize
						 position = debugProcess->DebuggerInstance->MemoryForImage - 1; //select last cell
				 }

				 int col = position % dataGridViewDebugMemory->Columns->Count;
				 int row = position / dataGridViewDebugMemory->Columns->Count;
				 
				 dataGridViewDebugMemory->CurrentCell = dataGridViewDebugMemory->Rows[row]->Cells[col];
				 dataGridViewDebugMemory->FirstDisplayedScrollingRowIndex = row;

				 dataGridViewDebugMemory->Rows[row]->Cells[col]->Selected = true;
			 }
		 }
private: void dataGridViewDebugMemory_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {

			 //incrment debug mem view size while scrolling
			 DebugCodeProcess ^ debugProcess;
			 DataGridView ^ dataGridViewMem = cli::safe_cast<DataGridView^ >(sender);
			 int frow = dataGridViewMem->FirstDisplayedScrollingRowIndex;

			 if(mainForm_IsDebugTask() && e->ScrollOrientation == ScrollOrientation::VerticalScroll 
									   && dataGridViewMem->Rows->Count
									   && frow == dataGridViewMem->Rows->Count - dataGridViewMem->DisplayedRowCount(true) 
									   && (e->Type == ScrollEventType::SmallIncrement || e->Type == ScrollEventType::LargeIncrement))
			 {
				 debugProcess = cli::safe_cast<DebugCodeProcess^ >(interpreterProcess);
				 
				 int old_mem_image_size = debugProcess->DebuggerInstance->MemoryForImage;
				 int increment_cells = old_mem_image_size / 10; //+ 10% 

				 debugProcess->DebuggerInstance->MemoryForImage = old_mem_image_size + increment_cells;
				 
				 if(old_mem_image_size != debugProcess->DebuggerInstance->MemoryForImage && 
					Convert::ToInt32(comboBoxThreadMemSelect->SelectedValue) == debugProcess->DebuggerInstance->CurrentThreadId)
				 {
					debugProcess->DebuggerInstance->LoadMemoryByThread();
					debugProcess->DebuggerInstance->LoadMemoryImageToView(dataGridViewMem);

					dataGridViewMem->FirstDisplayedScrollingRowIndex = frow;
				 }
			 }
		 }
	//funkcje operacji na tekscie
private: void editorRichTextBox_Cut(System::Object^  sender, System::EventArgs^  e) {
				 
				 WorkContextBroker::Instance->GetCurrentContext()->editorTextBox->richTextBox->Cut();
		}
private: void editorRichTextBox_Copy(System::Object^  sender, System::EventArgs^  e) {
			     
				 WorkContextBroker::Instance->GetCurrentContext()->editorTextBox->richTextBox->Copy();
	    }
private: void editorRichTextBox_Paste(System::Object^  sender, System::EventArgs^  e) {
			  
				 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
				
				 if(Clipboard::ContainsText())
				 {			
					 this->UseWaitCursor = true;
					 currentWorkContext->syntaxHighLighter->HighLightAndPaste(currentWorkContext->editorTextBox->richTextBox, 
																		      Clipboard::GetText(TextDataFormat::Text)->ToString());

					 this->UseWaitCursor = false;
				 }
		}
private: void editorRichTextBox_Undo(System::Object^  sender, System::EventArgs^  e) {
			     
				 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
				 
				 if(currentWorkContext->editorTextBox->CanUndo())
					currentWorkContext->editorTextBox->Undo();

				 mainForm_UpdateUI();
		}
private: void editorRichTextBox_Redo(System::Object^  sender, System::EventArgs^  e) {
			     
				 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
				 
				 if(currentWorkContext->editorTextBox->CanRedo())
					currentWorkContext->editorTextBox->Redo();

				 mainForm_UpdateUI();
		}
private: void editorRichTextBox_SelectAll(System::Object^  sender, System::EventArgs^  e) {
			    
				 WorkContextBroker::Instance->GetCurrentContext()->editorTextBox->richTextBox->SelectAll();
		}
private: void editorRestoreMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 
			     WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();

				 if(currentWorkContext) {
					(cli::safe_cast<ProjectFileContext^ >(currentWorkContext->fileContext))->Restore();

					this->UseWaitCursor = true;
					currentWorkContext->editorTextBox->richTextBox->Text = currentWorkContext->fileContext->Content;
					currentWorkContext->syntaxHighLighter->HighLightAll(currentWorkContext->editorTextBox->richTextBox);

					this->UseWaitCursor = false;
				 }
		 }
private: void editorInsertConstantMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 //show options menu
				 FormInsert ^ insertWindow = gcnew FormInsert();
				 insertWindow->Show();
		 }

private: void buttonOptionsAdvanced_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(buttonOptionsAdvanced->Text == cButtonAdvancedOptionsShowCaption) {
					 buttonOptionsAdvanced->Text = cButtonAdvancedOptionsHideCaption;
					 groupBoxAdvanced->Visible = true;
					 groupBoxIOOptions->Visible = true;
				 }
				 else {
					 buttonOptionsAdvanced->Text = cButtonAdvancedOptionsShowCaption;
					 groupBoxAdvanced->Visible = false;
					 groupBoxIOOptions->Visible = false;
				 }
	    }
private: void propertiesViewMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				 ToolStripMenuItem^ menuItem = cli::safe_cast<ToolStripMenuItem^ >(sender);

				 if(menuItem == propertiesViewMainMenuItem) {
					splitContainerMain->Panel2Collapsed = !propertiesViewMainMenuItem->Checked; 
				 }
				 else if(menuItem == outputViewMainMenuItem) {
					splitContainerEditor->Panel2Collapsed = !outputViewMainMenuItem->Checked; 
				 }
		}
private: void tabControlPages_Selected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e) {
			 
				 this->mainForm_ChangeContext(cli::safe_cast<TabPage^ >(e->TabPage));
		 }
private: void tabControlPages_Deselected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e) {

			     WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();

				 if(currentWorkContext) {
					currentWorkContext->settings->Save();
				 }
		 }
private: void textBoxOptionsMemorySize_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
				//accept only digits
				if(!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
					e->Handled = true;
				}
		 }

private: void interpreterProcess_Start(System::Object^  sender, System::EventArgs^  e) {
				
				InterpreterProcess ^ taskProcess = cli::safe_cast<InterpreterProcess^ >(sender);
				
				mainForm_UpdateUI();
				mainForm_ToggleInterface(taskProcess, false);
				mainForm_SetStatusBarInfo(taskProcess->ProcessWorkContext);
		}
private: void interpreterProcess_Complete(System::Object^  sender, System::EventArgs^  e) {
				 
				InterpreterProcess ^ taskProcess = cli::safe_cast<InterpreterProcess^ >(sender);
			 
				this->interpreterProcess = nullptr;

				mainForm_UpdateUI();
			    mainForm_ToggleInterface(taskProcess, true);
				mainForm_SetStatusBarInfo(taskProcess->ProcessWorkContext);

				mainForm_ReadLog();
		}
private: void debugProcess_Start(System::Object^  sender, System::EventArgs^  e) {
				//clear ui at debug end 
				RichTextBox ^ richTextBox = cli::safe_cast<InterpreterProcess^ >(sender)->ProcessWorkContext->editorTextBox->richTextBox;

				treeViewDebugThreadList->Nodes->Clear();
				treeViewDebugThreadList->Nodes->Add("Program is before first instruction");

				richTextBox->ReadOnly = true;
				richTextBox->Cursor = System::Windows::Forms::Cursors::Hand;
		}
private: void debugProcess_Complete(System::Object^  sender, System::EventArgs^  e) {
				//clear ui at debug end 
				RichTextBox ^ richTextBox = cli::safe_cast<InterpreterProcess^ >(sender)->ProcessWorkContext->editorTextBox->richTextBox;

				treeViewDebugThreadList->Nodes->Clear();

				if(e == nullptr)
					treeViewDebugThreadList->Nodes->Add(String::Format("Program exited with code {0}", 
														cli::safe_cast<DebugCodeProcess^ >(sender)->DebuggerInstance->DebugeeExitCode));

				richTextBox->ReadOnly = false;
				richTextBox->Cursor = System::Windows::Forms::Cursors::IBeam;
		}

private: void parseMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			    interpreterProcess = gcnew CodeParseProcess(false);	
				this->mainForm_RunTask();
		 }
private: void analyseMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				interpreterProcess = gcnew CodeAnalysisProcess(false);	
				this->mainForm_RunTask();
		 }
private: void runMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				
				interpreterProcess = RunProcessFactory::createRunProcess(false);	
				this->mainForm_RunTask();
		 }
private: void runSelectionMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				interpreterProcess = RunProcessFactory::createRunProcess(true);	
				this->mainForm_RunTask();
		 }
private: void runSeparateMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				interpreterProcess = gcnew RunCodeSeparateProcess(false);	
				this->mainForm_RunTask();
		 }
private: void debugMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			   
				interpreterProcess = gcnew DebugCodeProcess(this->Bounds, false);	

				interpreterProcess->OnStart += gcnew System::EventHandler(this, &FormMain::debugProcess_Start);
				interpreterProcess->OnComplete += gcnew System::EventHandler(this, &FormMain::debugProcess_Complete);
				this->mainForm_RunTask();

				this->tabControlRight->SelectedTab = this->tabPageDebug;
			    
		 }
private: void newfileMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				WorkContextBroker::Instance->AddPage();
				mainForm_UpdateUI();
		 }
private: void openfileMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				WorkContextBroker::Instance->OpenPage();
				mainForm_UpdateUI();
		 }
private: void savefileMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				////
				WorkContextBroker::Instance->SavePage();
		 }
private: void saveAsMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				WorkContextBroker::Instance->SavePageAs();
		 }
private: System::Void saveAllMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				WorkContextBroker::Instance->SaveAllPages();
		 }

private: void closeMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			    WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
				
			    if(currentWorkContext->fileContext->IsModified &&
				   MessageBox::Show( String::Format("File {0} is unsaved. Continue? ", currentWorkContext->fileContext->Title), "Unsaved changes", MessageBoxButtons::YesNo, MessageBoxIcon::Exclamation)
				   == System::Windows::Forms::DialogResult::No) 
				{
					return;
				}

				WorkContextBroker::Instance->RemovePage();

				//refresh recent menu
				uiMenuButtonGenerator::Generate(this->recentMainMenuItem, 
											GlobalOptions::Instance->RecentFilesList, 
											gcnew EventHandler(this, &FormMain::recentMainMenuItem_Click),
											true);


		 }

private: void saveAsProjectMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			    
			   WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
			   ProjectFileContext ^ projectFileContext = gcnew ProjectFileContext;

			   projectFileContext->settingsContent = currentWorkContext->settings;
			   currentWorkContext->fileContext = projectFileContext;
			   this->saveAsMainMenuItem_Click(sender, e);
		 }

private: void stopMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			 if(interpreterProcess) {
				 interpreterProcess->Stop();
			 }
		 }

private: void detachMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			 if(mainForm_IsDebugTask()) {
				 cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->Detach();
			 }
		 }

		 //On error list click, set cursor at given error in sourcecode
private: void listViewOutputErrors_DoubleClick(System::Object^ sender, System::EventArgs^  e) {

			 ListView ^ listView = cli::safe_cast<ListView^ >(sender);
			 ListViewItem^ selectedListViewItem = listView->SelectedItems[0];
			 
			 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
			 currentWorkContext->editorTextBox->richTextBox->SelectionStart = 
				 currentWorkContext->outputLister->GetErrorPosition(selectedListViewItem);

			 currentWorkContext->editorTextBox->richTextBox->ScrollToCaret();
			 currentWorkContext->editorTextBox->richTextBox->Focus();
		 }

private: void buttonOptionsDefault_Click(System::Object^  sender, System::EventArgs^  e) {

			 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
			 currentWorkContext->settings->Default();
             currentWorkContext->settings->Load();
		 }
private: void optionsMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 FormOptions ^ optionsWindow = gcnew FormOptions();
			 optionsWindow->ShowDialog(this);

			 mainForm_UpdateUI();
			 //optionsWindow->Dispose();
		 }
private: void recentMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 ToolStripMenuItem ^ senderMenuItem = cli::safe_cast<ToolStripMenuItem^ >(sender);

			 FileContext ^ recentFileContext = FileContextFactory::createFileContext(senderMenuItem->Text);
			 if(recentFileContext->Open())
			 {
				WorkContextBroker::Instance->OpenPage(recentFileContext);
			 }
		 }
private: void comboBoxOptionsLanguage_SelectionChangeCommitted(System::Object^  sender, System::EventArgs^  e) {

			 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();

			 if(currentWorkContext)
			 {
				if(mainForm_IsDebugTask() && interpreterProcess->Working() && currentWorkContext == interpreterProcess->ProcessWorkContext) {
					MessageBox::Show("You cannot change the language while debugging.", "Editor is read only", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
					
					currentWorkContext->settings->Load(); //rollback control selection
					return;
				}
				 
				currentWorkContext->settings->Save();
				currentWorkContext->syntaxHighLighter = SyntaxHighLighterFactory::createSyntaxHighLighter( currentWorkContext->settings->GetLanguage() );

				this->UseWaitCursor = true;

			    currentWorkContext->syntaxHighLighter->HighLightAll(currentWorkContext->editorTextBox->richTextBox);

				this->UseWaitCursor = false;

				mainForm_SetStatusBarInfo(currentWorkContext);
			 }
			 mainForm_UpdateUI();
		 }
private: void contextMenuButtonLogRefresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 mainForm_ReadLog(GlobalOptions::Instance->LogPath);
		 }
private: void contextMenuButtonLogOpen_Click(System::Object^  sender, System::EventArgs^  e) {
			 OpenFileDialog ^ openFileDialog = gcnew OpenFileDialog();

			 openFileDialog->Filter = cLogOpenDialogFilters;
			 openFileDialog->Title = "Select a file to open";

			 if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK){
				mainForm_ReadLog(openFileDialog->FileName);
			 }
		 }

private: void listViewOutputErrors_ColumnWidthChanged(System::Object^  sender, System::Windows::Forms::ColumnWidthChangedEventArgs^  e) {
			 
			 if(this->listViewOutputErrors->Items->Count == 0){
					this->tabPageErrorList->Text = "Message List";
			 }
			 else{
					this->tabPageErrorList->Text = String::Format("Message List ({0})", this->listViewOutputErrors->Items->Count);
			 }
		 }

private: System::Void buttonDebugStep_Click(System::Object^  sender, System::EventArgs^  e) {

			 if(mainForm_IsDebugTask() && interpreterProcess->Working())
			 {
				 this->UseWaitCursor = true;
				 cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->Step();
				 this->UseWaitCursor = false;
				 
				 mainForm_RunDebugTask();
			 }
			 else
				 debugMainMenuItem_Click(sender, e);

		 }
private: System::Void buttonDebugRunToCursor_Click(System::Object^  sender, System::EventArgs^  e) {

			 if(mainForm_IsDebugTask() && interpreterProcess->Working())
			 {
				 this->UseWaitCursor = true;
				 cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->RunToCursor();
				 this->UseWaitCursor = false;

				 mainForm_RunDebugTask();
			 }

		 }
private: System::Void buttonDebugStepOver_Click(System::Object^  sender, System::EventArgs^  e) {
			 //step over click
			 if(mainForm_IsDebugTask() && interpreterProcess->Working())
			 {
				 this->UseWaitCursor = true;
				 cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->StepOver();
				 this->UseWaitCursor = false;

				 mainForm_RunDebugTask();
			 }
		 }
private: System::Void buttonDebugPlaceCursor_Click(System::Object^  sender, System::EventArgs^  e) {
			 //place cursor click
			 
			 if(mainForm_IsDebugTask() && interpreterProcess->Working())
			 {
				 debugForm_MemoryScrollToPosition( cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->DebuggerInstance->MemoryPosition );
			 }
		 }
private: System::Void contextMenuButtonScrollToMemPos_Click(System::Object^  sender, System::EventArgs^  e) {
			 //scroll to memory pos
			 if(mainForm_IsDebugTask())
			 {
				 debugForm_MemoryScrollToPosition( cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->DebuggerInstance->MemoryPosition[Convert::ToInt32(comboBoxThreadMemSelect->SelectedValue) ]);
			 }
		 }
private: System::Void contextMenuButtonScrollToLNCell_Click(System::Object^  sender, System::EventArgs^  e) {
			 //scroll to last non zero pos
			 if(mainForm_IsDebugTask())
			 {
				 debugForm_MemoryScrollToPosition(cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->DebuggerInstance->MemoryLNZPos[ Convert::ToInt32(comboBoxThreadMemSelect->SelectedValue) ]);
			 }
		 }
private: void contextMenuButtonMemoryRepresentation_Click(System::Object^  sender, System::EventArgs^  e) {
			//custom event, change debug memory table output p.ex hex 

			 DebugCodeProcess ^ debugProcess;
			 ToolStripMenuItem ^ sender_menu = cli::safe_cast<ToolStripMenuItem ^>(sender);

			 if(mainForm_IsDebugTask())		
			 {
				 debugProcess = cli::safe_cast<DebugCodeProcess^ >(interpreterProcess);
				 
				 if(sender_menu->Text == cButtonMemoryReprCharCaption)
					debugProcess->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory, gcnew CharacterCellPrinter());
				 else if(sender_menu->Text == cButtonMemoryReprHexCaption)
					debugProcess->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory, gcnew HexCellPrinter());
				 else
					debugProcess->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory, gcnew MemoryCellPrinter());
			 }
		 }

private: void contextMenuButtonChangeMemTabColumnNumber_Click(System::Object^  sender, System::EventArgs^  e) {
			//custom event, change debug memory table columns number
			 ToolStripMenuItem ^ sender_menu = cli::safe_cast<ToolStripMenuItem ^>(sender);

			 if(mainForm_IsDebugTask())
			 {   //captions passed as int, other text = 0 
				 if(sender_menu->Text == cButtonMemoryTabColsAutoCaption) {
					 cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory, Int16(0));
				 }
				 else
					 cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory, Convert::ToInt16(sender_menu->Text));
			 }
		 }

private: System::Void treeViewDebugThreadList_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
			 //click on treeview to show memory
			 DebugCodeProcess ^ debugProcess;

			 if(mainForm_IsDebugTask())
			 {    
				 debugProcess = cli::safe_cast<DebugCodeProcess^ >(interpreterProcess);
				 if(e->Node->Text->Contains("Thread") && GlobalOptions::Instance->MemViewOnThreadCaptionClick)
				 {
					 debugProcess->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory, cli::safe_cast<int>(e->Node->Tag));

					 debugForm_MemoryScrollToPosition(debugProcess->DebuggerInstance->MemoryPosition[ cli::safe_cast<int>(e->Node->Tag) ]);
					 comboBoxThreadMemSelect->SelectedIndex = cli::safe_cast<TreeView ^>(sender)->Nodes->IndexOf(e->Node);
				 } 
			 }
			 
		 }

private: System::Void comboBoxThreadMemSelect_SelectionChangeCommitted(System::Object^  sender, System::EventArgs^  e) {
			 //click on combobox to show memory
			 int sel_threadId = Convert::ToInt32(comboBoxThreadMemSelect->SelectedValue);
			 
			 if(mainForm_IsDebugTask())
			 {    
				 cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->DebuggerInstance->LoadMemoryImageToView(dataGridViewDebugMemory, sel_threadId);
			 }
		 }

private: System::Void contextMenuButtonExpandAll_Click(System::Object^  sender, System::EventArgs^  e) {

			 DebugTreeViewExpander ^ treeViewExpander = gcnew DebugTreeViewExpander(this->treeViewDebugThreadList);

			 if(sender == contextMenuButtonExpandAll) {
				 treeViewExpander->ExpandAll();
			 }			 
			 else if(sender == contextMenuButtonCollapseAll) {
				 treeViewExpander->CollapseAll();
			 }	
			 else if(sender == contextMenuButtonCollapseTree) {
				 treeViewExpander->CollapseAllThreads();
			 }	
		 }
private: System::Void contextMenuButtonTrackThread_Click(System::Object^  sender, System::EventArgs^  e) {
			 //click on thread node to trace it
			 
			 if(mainForm_IsDebugTask() && treeViewDebugThreadList->SelectedNode->Text->Contains("Thread"))
			 {    
				 cli::safe_cast<DebugCodeProcess^ >(interpreterProcess)->DebuggerInstance->ThreadTracing[ cli::safe_cast<int>(treeViewDebugThreadList->SelectedNode->Tag) ] = contextMenuButtonTrackThread->Checked;
			 }
		 }
private: System::Void treeViewDebugThreadList_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

			 // Show menu only if the right mouse button is clicked.
			 if (mainForm_IsDebugTask() && e->Button == System::Windows::Forms::MouseButtons::Right)
			 {
				 Point p = Point(e->X, e->Y);
				 TreeNode ^ picked_node = treeViewDebugThreadList->GetNodeAt(p);

				 DebugCodeProcess ^ debugProcess = cli::safe_cast<DebugCodeProcess^ >(interpreterProcess);

				 contextMenuButtonTrackThread->Visible = false; 
				 toolStripSeparator7->Visible = false; 

				 if (picked_node && debugProcess)
				 {
					 treeViewDebugThreadList->SelectedNode = picked_node;

					 if(picked_node->Text->Contains("Thread") )
					 {
						 contextMenuButtonTrackThread->Checked = debugProcess->DebuggerInstance->ThreadTracing[ cli::safe_cast<int>(picked_node->Tag) ];
						 contextMenuButtonTrackThread->Visible = true; 
						 toolStripSeparator7->Visible = true; 
					 }
				 }
			}
		 }

private: System::Void textBoxOutput_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 //scroll to end text in output
			 
			 textBoxOutput->SelectionStart = textBoxOutput->Text->Length;
			 textBoxOutput->ScrollToCaret();
			 //textBoxOutput->ScrollToEnd();
		 }
private: void pluginsMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 //run plugin
			 
			 String ^ code;
			 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
			 Plugin ^ currentPlugin = GlobalOptions::Instance->Plugins->GetPlugin( cli::safe_cast<ToolStripMenuItem ^>(sender)->Text );

			 if(currentWorkContext && currentPlugin)
			 {
				this->UseWaitCursor = true;

				code = currentWorkContext->editorTextBox->richTextBox->SelectionLength > 0 ? currentWorkContext->editorTextBox->richTextBox->SelectedText
																							: currentWorkContext->editorTextBox->richTextBox->Text;

				if(mainForm_IsDebugTask()) { //execute only plugins who dont return the code
					if(currentPlugin->OnInvoke(code, currentWorkContext->settings->GetLanguage())->Length > 0)
						MessageBox::Show( String::Format("Plugin {0} cannot modify the code while debugging.\nUse a pragma instead", currentPlugin->Name()), "Editor is read only", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
						
				}
				else
				{
					currentWorkContext->syntaxHighLighter->HighLightAndPaste(currentWorkContext->editorTextBox->richTextBox, 
																			 currentPlugin->OnInvoke(code, currentWorkContext->settings->GetLanguage())
																			);
				}
				
				this->UseWaitCursor = false;
			 }
		 }
private: void editorInsertPluginMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 //insert empty pragma of selected plugin
			 
			 WorkContext ^ currentWorkContext = WorkContextBroker::Instance->GetCurrentContext();
			 Plugin ^ currentPlugin = GlobalOptions::Instance->Plugins->GetPlugin( cli::safe_cast<ToolStripMenuItem ^>(sender)->Text );

			 if(currentWorkContext && currentPlugin) {	
				 currentWorkContext->syntaxHighLighter->HighLightAndPaste(currentWorkContext->editorTextBox->richTextBox,
					 String::Format("@@@{0} {1}@@@", currentPlugin->Name(), 
													 currentWorkContext->editorTextBox->richTextBox->SelectionLength > 0 ? currentWorkContext->editorTextBox->richTextBox->SelectedText : "pragma commands"));
			 }
		 }

private: System::Void protipMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 //show protip or bt info window
			 FormProtip ^ protip = gcnew FormProtip();
			 
			 if(sender == this->aboutBTMainMenuItem) {
				 protip->behaveAsLangInfoForm();
			 }
			 else { 
				 protip->getNextProtip();
			 }

			 protip->ShowDialog(this);
		 }
private: System::Void aboutMainMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 //about msbox
			 MessageBox::Show( String::Format("Brainthread IDE\nonehundredfifteen 2018\nVersion {0}\nDebugger version {1}", "0.9", "0.2c"), "About", MessageBoxButtons::OK, MessageBoxIcon::Information);
		 }
private: System::Void tabControlPages_DragOver(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			//change tab order
            Point pt = this->tabControlPages->PointToClient( Point(e->X, e->Y));
            
            if(this->GetTabPageByCursorLocation(pt) && e->Data->GetDataPresent(TabPage::typeid)) {
				e->Effect = DragDropEffects::Move; 
            }
            else 
                e->Effect = DragDropEffects::None;
		 }
private: System::Void tabControlPages_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {

			Point pt = this->tabControlPages->PointToClient( Point(e->X, e->Y));
            TabPage ^ drag_tab, ^ hover_tab = this->GetTabPageByCursorLocation(pt);

            if(hover_tab && e->Data->GetDataPresent(TabPage::typeid))
            {
                int drop_location_index = this->tabControlPages->TabPages->IndexOf(hover_tab);
				
                TabPage ^ drag_tab = cli::safe_cast<TabPage ^>(e->Data->GetData(TabPage::typeid));

                if(this->tabControlPages->TabPages->IndexOf(drag_tab) != drop_location_index) 
				{
					List<TabPage ^> ^ pages = gcnew List<TabPage ^>; 
                    //Put all tab pages into an array.
                    for each (TabPage ^ page in this->tabControlPages->TabPages) {
                        //Except the one we are dragging.
                        if(page != drag_tab)
                           pages->Add(drag_tab);
                    }

                    //Now put the one we are dragging it at the proper location.
                    pages->Insert(drop_location_index, drag_tab);

                    //Add them all back in.
					this->tabControlPages->TabPages->Clear();
                    this->tabControlPages->TabPages->AddRange(pages->ToArray());

                    //SelectedTab = drag_tab
                }
            }
		 }
private: System::Void tabControlPages_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {

			/*if(e->Data->GetDataPresent(TabPage::typeid))
            {
				e->Effect = DragDropEffects::Move; 
            }
            else {
                e->Effect = DragDropEffects::None;
            }*/
		 }
private: System::Void tabControlPages_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

            TabPage ^ drag_tab = this->GetTabPageByCursorLocation( Point(e->X, e->Y));

            if(drag_tab) {
				this->tabControlPages->DoDragDrop(drag_tab, static_cast<DragDropEffects>(DragDropEffects::All | DragDropEffects::Move));
            }
		 }
private: System::Void FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
		  //form closing
			 GlobalOptions::Instance->SaveToFile();
		 }
};
}

