#pragma once

#include "SyntaxHighLighter.h"
#include "OutputLister.h"

#include "options/PageSettings.h"
#include "file_io/FileContext.h"
#include "file_io/ProjectFileContext.h"


namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Windows::Forms;

	ref class FormMain;

	public ref class WorkContext
	{
	public:	
			EditorTextBox::EditorTextBoxControl ^ editorTextBox;
			DefaultSyntaxHighLighter ^ syntaxHighLighter;
			PageSettings ^ settings;
			FileContext ^ fileContext;
			OutputLister ^ outputLister;
	public:
			~WorkContext();
	};

	public ref class WorkContextBroker
	{
	protected:
		  WorkContextBroker() {}
		  WorkContextBroker(const WorkContextBroker%) { throw gcnew System::InvalidOperationException("singleton cannot be copy-constructed"); }
		  
    public:
			static property WorkContextBroker^ Instance
			{
				WorkContextBroker^ get() { return m_Instance; }
			}
			
	private:
			static WorkContextBroker^ m_Instance = gcnew WorkContextBroker;
	    
	public:
			void Initialize(FormMain^ w, TabControl^ t);

#ifdef _DEBUG
			bool isNotinitialized(){
				return this->Owner == nullptr;
			}
#endif

	private:
			TabControl^  Owner;
			BrainthreadIDE::FormMain^ Window;
			
			System::Collections::Generic::Dictionary<TabPage^, WorkContext^>^ contextPairs;

			void InitializeContext(WorkContext ^ context);
			void AddContext(TabPage ^ newTabPage, WorkContext ^ newWorkContext);
			void SaveContent(WorkContext ^ saveWorkContext);
			TabPage ^ IsAlreadyOpened(FileContext ^ fileContext);

	public:
			WorkContext ^ GetCurrentContext();
			WorkContext ^ GetContext(TabPage ^ tabPage);
			TabPage ^ GetPage(WorkContext ^ context);

			void AddPage(void);
			void OpenPage(void);
			void OpenPage(FileContext ^ openFileContext);
			void RemovePage(void);

			void SavePage(void);
			void SavePageAs(void);
			void SaveAllPages(void);

			void RefreshTabTitle(TabPage ^ tabPage);
			void RefreshEditor();
	private:
			void RefreshEditor(WorkContext ^ workContext);

			
	};
}