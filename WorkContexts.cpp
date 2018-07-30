#include "StdAfx.h"
#include "WorkContexts.h"
#include "factories/FileContextFactory.h"
#include "factories/SyntaxHighlighterFactory.h"
#include "FormMain.h"


namespace BrainthreadIDE 
{
	void WorkContextBroker::Initialize(FormMain^ w, TabControl^ t)
	{
		Owner = t;
		Window = w;

		contextPairs = gcnew System::Collections::Generic::Dictionary<TabPage^, WorkContext^>();
	}

	WorkContext ^ WorkContextBroker::GetCurrentContext()
	{
		return Owner->SelectedTab != nullptr ? contextPairs[ Owner->SelectedTab ] : nullptr;
	}

	WorkContext ^ WorkContextBroker::GetContext(TabPage ^ tabPage)
	{
		if(tabPage && contextPairs->ContainsKey(tabPage))
		{
			return contextPairs[ tabPage ];
		}
		return nullptr;
	}

	TabPage ^ WorkContextBroker::GetPageByContext(WorkContext ^ context)
	{	//get WorkContext tab
		for each( KeyValuePair<TabPage^, WorkContext^>^ kvp in contextPairs)
		{
			if(kvp->Value == context)
				return kvp->Key;
		}
		return nullptr;
	}

	TabPage ^ WorkContextBroker::GetPageByCursorLocation(System::Drawing::Point pt) 
	{
			//for drag n drop - fins tab page over cursor
			for(int i = 0; i < this->Owner->TabPages->Count; ++i) {
				if(this->Owner->GetTabRect(i).Contains(pt)) {
					return this->Owner->TabPages[i];
				}
			}
    
			return nullptr;
	}

	void WorkContextBroker::saveContent(WorkContext ^ saveWorkContext)
	{
		saveWorkContext->fileContext->Content = saveWorkContext->editorTextBox->richTextBox->Text;
	}

	void WorkContextBroker::initializeContext(WorkContext ^ context)
	{
		//editor
		EditorTextBox::EditorTextBoxControl ^ newEditorTextBox = gcnew EditorTextBox::EditorTextBoxControl;

		newEditorTextBox->Location = System::Drawing::Point(0, 0);
		newEditorTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
		newEditorTextBox->Name = String::Concat(L"editor_richbox_", (Owner->TabCount + 1).ToString());
		newEditorTextBox->GutterMargin = 10;
		newEditorTextBox->TabIndent = true;
		newEditorTextBox->richTextBox->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
		newEditorTextBox->richTextBox->Text = L"";

		newEditorTextBox->richTextBox->AcceptsTab = true;
		newEditorTextBox->richTextBox->ContextMenuStrip = Window->editorContextMenu;

		newEditorTextBox->richTextBox->TextChanged += gcnew System::EventHandler(Window, &FormMain::editorRichTextBox_TextChanged);
		newEditorTextBox->richTextBox->SelectionChanged += gcnew System::EventHandler(Window, &FormMain::editorRichTextBox_SelectionChanged);
		
		//other
		context->editorTextBox = newEditorTextBox;
		context->syntaxHighLighter = SyntaxHighLighterFactory::createSyntaxHighLighter(gcnew BrainthreadSyntaxHighLighter);
		context->settings = gcnew PageSettings;
		context->fileContext = gcnew CodeFileContext;

		context->outputLister = gcnew OutputLister(Window->listViewOutputErrors, Window->textBoxOutput);
		context->outputLister->Refresh();
	}

	void WorkContextBroker::addContext(TabPage ^ newTabPage, WorkContext ^ newWorkContext)
	{
		//adding
		newTabPage->Controls->Add(newWorkContext->editorTextBox);
		Owner->TabPages->Add(newTabPage);

		//coupling
        contextPairs->Add(newTabPage, newWorkContext);
		Owner->SelectedTab = newTabPage;

		//settings
		newWorkContext->settings->Load();
	
		//title
		newTabPage->Text = newWorkContext->fileContext->Title;
	}

	void WorkContextBroker::AddPage(void)
	{
		TabPage^ newTabPage = gcnew TabPage;
		WorkContext ^ newWorkContext = gcnew WorkContext;

		this->initializeContext(newWorkContext);
		this->addContext(newTabPage, newWorkContext);

		this->RefreshEditor(newWorkContext);
	}

	void WorkContextBroker::OpenPage(void)
	{
		FileContext ^ openFileContext = FileContextFactory::createFileContext();

		if(openFileContext != nullptr && openFileContext->Open())
		{
			this->OpenPage(openFileContext);
		}
	}

	void WorkContextBroker::OpenPage(FileContext ^ openFileContext)
	{
		TabPage^ newTabPage; 
		WorkContext ^ newWorkContext = gcnew WorkContext;
		WorkContext ^ previousWorkContext = GetCurrentContext();

		newTabPage = this->IsAlreadyOpened(openFileContext);
		if(newTabPage)
		{
			MessageBox::Show( String::Format("File {0} is already opened!", openFileContext->FilePath), "Open error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			Owner->SelectedTab = newTabPage;
			return;
		}

		//tab creation
		newTabPage = gcnew TabPage;

		this->initializeContext(newWorkContext);
		newWorkContext->fileContext = openFileContext;
		newWorkContext->editorTextBox->richTextBox->Text = openFileContext->Content;

		if(newWorkContext->IsProjectContext) //change highlither as in  projets options
		{
			newWorkContext->settings = (cli::safe_cast<ProjectFileContext^ >(openFileContext))->settingsContent;
			newWorkContext->syntaxHighLighter = SyntaxHighLighterFactory::createSyntaxHighLighter( newWorkContext->settings->GetLanguage() );
			newWorkContext->settings->Load();
		}

		//commit new page 
		this->addContext(newTabPage, newWorkContext);

		//if only one empty page + just added page, replace instead
		if(this->contextPairs->Count == 2 && false == previousWorkContext->fileContext->HasPhysicalFile()
			&& String::IsNullOrEmpty(previousWorkContext->editorTextBox->richTextBox->Text) && false == previousWorkContext->IsProjectContext) 
		{
			this->RemovePage(this->GetPageByContext(previousWorkContext));
		}
		
		//draw new page
		this->RefreshEditor(newWorkContext);
	}

	void WorkContextBroker::RemovePage(void)
	{
		RemovePage(Owner->SelectedTab);
	}

	void WorkContextBroker::RemovePage(TabPage ^ tabPage)
	{
		Owner->TabPages->Remove(tabPage);

		this->AddFileToHistory( contextPairs[tabPage] );

		delete contextPairs[tabPage]->fileContext;
		delete contextPairs[tabPage];

		contextPairs->Remove(tabPage);

		if(Owner->TabPages->Count < 1) {
			this->AddPage();
		}
	}

	void WorkContextBroker::SavePage(void)
	{
		WorkContext ^ currentWorkContext = GetCurrentContext();

		this->saveContent(currentWorkContext);
		currentWorkContext->fileContext->Save();
			
		this->refreshTabTitle(nullptr);
	}

	void WorkContextBroker::SavePageAs(void)
	{
		WorkContext ^ currentWorkContext = this->GetCurrentContext();

		this->saveContent(currentWorkContext);
		(cli::safe_cast<CodeFileContext^ >(currentWorkContext->fileContext))->SaveAs();
			
		this->refreshTabTitle(nullptr);
	}

	void WorkContextBroker::SaveAllPages(void)
	{
		for each( KeyValuePair<TabPage^, WorkContext^>^ kvp in contextPairs)
		{
			this->saveContent(kvp->Value);
			kvp->Value->fileContext->Save();

			this->refreshTabTitle(kvp->Key);
		}
	}

	void WorkContextBroker::ClonePage(void)
	{
		WorkContext ^ newWorkContext;
		WorkContext ^ clonedWorkContext = GetCurrentContext();

		this->AddPage();

		//fill with source
		newWorkContext = GetCurrentContext(); //!

		newWorkContext->editorTextBox->richTextBox->Rtf = clonedWorkContext->editorTextBox->richTextBox->Rtf;
		newWorkContext->settings = gcnew PageSettings(clonedWorkContext->settings);
	}

	void WorkContextBroker::DiscardOtherPages(void)
	{
		List<TabPage ^> ^ pages = gcnew List<TabPage ^>;
		TabPage ^ pageToSave = this->GetPageByContext(this->GetCurrentContext());

		//populate tmp list
		for each(TabPage ^ page in Owner->TabPages)
			pages->Add(page);

		//discard all without current tab
		for each(TabPage ^ page in pages)
		{
			if(page != pageToSave) {
				this->RemovePage(page);
			}
		}
	}

	//other methods

	TabPage ^ WorkContextBroker::IsAlreadyOpened(FileContext ^ fileContext)
	{
		for each( KeyValuePair<TabPage^, WorkContext^>^ kvp in contextPairs)
		{
			if(String::Compare(kvp->Value->fileContext->FilePath, fileContext->FilePath) == 0)
				return kvp->Key;
		}
		return nullptr;
	}

	void WorkContextBroker::refreshTabTitle(TabPage ^ tabPage)
	{
		WorkContext ^ chosenWorkContext;
		
		if(tabPage == nullptr || contextPairs->ContainsKey(tabPage) == false)
			tabPage = Owner->SelectedTab;

		chosenWorkContext = contextPairs[ tabPage ];
		tabPage->Text = chosenWorkContext->fileContext->Title;
	}

	void WorkContextBroker::RefreshEditor(WorkContext ^ workContext)
	{
		workContext->editorTextBox->richTextBox->Font = GlobalOptions::Instance->EditorFont;
		workContext->editorTextBox->richTextBox->ForeColor = GlobalOptions::Instance->EditorForeColor;
		workContext->editorTextBox->richTextBox->BackColor = GlobalOptions::Instance->EditorBackColor;
		workContext->editorTextBox->GutterForeColor = GlobalOptions::Instance->MarginForeColor;
		workContext->editorTextBox->GutterBackColor = GlobalOptions::Instance->MarginBackColor;

		if(workContext->editorTextBox->richTextBox->Text->Length > 0)
		{
			workContext->syntaxHighLighter->HighLightAll(workContext->editorTextBox->richTextBox);
			workContext->editorTextBox->ClearUndoHistory();
		}
	}

	void WorkContextBroker::RefreshEditor()
	{
		Dictionary<TabPage^, WorkContext^>::Enumerator etor = contextPairs->GetEnumerator();

		while(etor.MoveNext())
		{
			etor.Current.Value->syntaxHighLighter = SyntaxHighLighterFactory::createSyntaxHighLighter( etor.Current.Value->settings->GetLanguage() );
			this->RefreshEditor(etor.Current.Value);
		}
	}

	void WorkContextBroker::AddFileToHistory(WorkContext ^ workContext)
	{	//WorkContext save to history
		if(workContext->fileContext && workContext->fileContext->HasPhysicalFile())
		{
			if(workContext->IsProjectContext)
				GlobalOptions::Instance->RecentFilesListAdd(cli::safe_cast<ProjectFileContext^ >(workContext->fileContext)->ProjectPath);
			else
				GlobalOptions::Instance->RecentFilesListAdd(workContext->fileContext->FilePath);
		}
	}

	void WorkContextBroker::AddFilesToHistory()
	{
		Dictionary<TabPage^, WorkContext^>::Enumerator etor = contextPairs->GetEnumerator();

		while(etor.MoveNext()) {
			this->AddFileToHistory(etor.Current.Value);
		}
	}

	/*void FormMain::editorPagesContainer::AddMenuToEditor(RichTextBox^ richtextBox)
	{
		System::Windows::Forms::ContextMenuStrip ^ contextMenu = gcnew System::Windows::Forms::ContextMenuStrip ;

		//undo
		ToolStripMenuItem^ menuItem = gcnew ToolStripMenuItem("Undo", 
															  nullptr, 
															  gcnew EventHandler(Window, &FormMain::editorRichTextBox_Undo),
															  System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Z);
		contextMenu->Items->Add(menuItem);
		
		//redo
		menuItem = gcnew ToolStripMenuItem("Redo", 
											nullptr, 
											gcnew EventHandler(Window, &FormMain::editorRichTextBox_Redo),
											System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Y);
		menuItem->Enabled = false;
		contextMenu->Items->Add(menuItem);

		//separator
		ToolStripSeparator^ menuSeparator = gcnew ToolStripSeparator();
		contextMenu->Items->Add(menuSeparator);

		//cut
		menuItem = gcnew ToolStripMenuItem("Cut", 
											nullptr, 
											gcnew EventHandler(Window, &FormMain::editorRichTextBox_Cut),
											System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::X);
		contextMenu->Items->Add(menuItem);

		//copy
		menuItem = gcnew ToolStripMenuItem("Copy", 
											nullptr, 
											gcnew EventHandler(Window, &FormMain::editorRichTextBox_Copy),
											System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::C);
		contextMenu->Items->Add(menuItem);

		//paste
		menuItem = gcnew ToolStripMenuItem("Paste", 
											nullptr, 
											gcnew EventHandler(Window, &FormMain::editorRichTextBox_Paste),
											System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::V);
		contextMenu->Items->Add(menuItem);

		//separator
		menuSeparator = gcnew ToolStripSeparator();
		contextMenu->Items->Add(menuSeparator);

		//select all
		menuItem = gcnew ToolStripMenuItem("Select all", 
											nullptr, 
											gcnew EventHandler(Window, &FormMain::editorRichTextBox_SelectAll),
											System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::A);
		contextMenu->Items->Add(menuItem);
       
        richtextBox->ContextMenuStrip = contextMenu;
	}*/

}
