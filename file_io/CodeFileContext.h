#pragma once

#include "FileContext.h"

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::IO;
	using namespace System::Windows::Forms;

	public ref class CodeFileContext : public FileContext
	{
	public:
		CodeFileContext(void){}
		CodeFileContext(String ^ filepath) : FileContext(filepath){}

		virtual bool Open() override
		{
			  if(FileContext::Open() == false)
			  {
				  MessageBox::Show( String::Format("Cannot open the file: {0}", this->filePath), "Cannot open the file", MessageBoxButtons::OK, MessageBoxIcon::Error);
				  return false;
			  }

			  return true;
		}

		virtual bool Save() override
		{
			  if(this->HasPhysicalFile() == false)
				  return this->SaveAs();

			  return FileContext::Save();
		}

		virtual bool SaveAs()
		{
			  SaveFileDialog ^ saveFileDialog = gcnew SaveFileDialog();

			  saveFileDialog->Filter = cDialogFilters;
			  saveFileDialog->FilterIndex = 1;
			  saveFileDialog->Title = "Save file as";
			  saveFileDialog->RestoreDirectory = true;

			  if(this->filePath)
			  {
				  saveFileDialog->FileName = Path::GetFileName(this->filePath);
				  saveFileDialog->InitialDirectory = Path::GetDirectoryName(this->filePath);
			  }
			  //else
				//saveFileDialog->InitialDirectory = Environment->GetFolderPath(Environment::SpecialFolder::MyDocuments);

			  if (saveFileDialog->ShowDialog() == DialogResult::OK)
			  {
					this->filePath = saveFileDialog->FileName;
					this->fileName = Path::GetFileName(saveFileDialog->FileName);

					return FileContext::Save();
			  }

			  return false;
		}

	private: 
		literal String ^ cDialogFilters = "Brain*** sourcefiles|*.b;*.bf;*.bt|Brainthread files|*.bt|Brainfuck files|*.b;*.bf|pBrain files|*.pb|All files|*.*";

	};
}

