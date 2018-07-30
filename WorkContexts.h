#pragma once

#include "WorkContext.h"

//#include "SyntaxHighLighter.h"
//#include "OutputLister.h"

//#include "options/PageSettings.h"
//#include "file_io/FileContext.h"
//#include "file_io/ProjectFileContext.h"


namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Windows::Forms;
	ref class FormMain;

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

			void initializeContext(WorkContext ^ context);
			void addContext(TabPage ^ newTabPage, WorkContext ^ newWorkContext);
			void saveContent(WorkContext ^ saveWorkContext);

			TabPage ^ IsAlreadyOpened(FileContext ^ fileContext);
			void refreshTabTitle(TabPage ^ tabPage);

	public:
			WorkContext ^ GetCurrentContext();
			WorkContext ^ GetContext(TabPage ^ tabPage);
			TabPage ^ GetPageByContext(WorkContext ^ context);
			TabPage ^ GetPageByCursorLocation(System::Drawing::Point pt);

			void AddPage(void);
			void OpenPage(void);
			void OpenPage(FileContext ^ openFileContext);

			void RemovePage(void);
			void RemovePage(TabPage ^ tabPage);

			void ClonePage(void);
			void DiscardOtherPages(void);

			void SavePage(void);
			void SavePageAs(void);
			void SaveAllPages(void);

			void RefreshEditor();
			void AddFilesToHistory();

	private:
			void RefreshEditor(WorkContext ^ workContext);
			void AddFileToHistory(WorkContext ^ workContext);

			
	};
}