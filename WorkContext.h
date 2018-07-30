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

	public ref class WorkContext
	{
	public:	WorkContext()
			{
			}

	public:	WorkContext(WorkContext ^ copy) : fileContext(copy->fileContext), settings(copy->settings)
			{
				this->editorTextBox = copy->editorTextBox;
				this->outputLister = copy->outputLister;
				this->syntaxHighLighter = copy->syntaxHighLighter;
			}

	public:	
			EditorTextBox::EditorTextBoxControl ^ editorTextBox;
			DefaultSyntaxHighLighter ^ syntaxHighLighter;
			PageSettings ^ settings;
			FileContext ^ fileContext;
			OutputLister ^ outputLister;

	public: property bool IsProjectContext {		
				bool get() {
					return (this->fileContext->GetType() == ProjectFileContext::typeid);
				}
			}
			property bool SourceNotSaved {		
				bool get() {

					return (this->fileContext->HasPhysicalFile() == false ? String::IsNullOrEmpty(editorTextBox->richTextBox->Text) == false : 
						   String::Compare(this->fileContext->Content, editorTextBox->richTextBox->Text) != 0);	   
				}
			}
	};
}