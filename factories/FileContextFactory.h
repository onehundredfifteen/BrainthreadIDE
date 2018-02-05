#pragma once

#include "../file_io/ProjectFileContext.h"	

namespace BrainthreadIDE 
{
	public ref class FileContextFactory 
	{
	public: static FileContext ^ createFileContext() 
			{
				OpenFileDialog ^ openFileDialog = gcnew OpenFileDialog();

				openFileDialog->Filter = cDialogFilters;
				openFileDialog->Title = "Select a file to open";
				openFileDialog->RestoreDirectory = true;

				if (openFileDialog->ShowDialog() == DialogResult::OK)
				{
					if(String::Compare(Path::GetExtension(openFileDialog->FileName), cIDEProjExt) == 0) {
						return gcnew ProjectFileContext(openFileDialog->FileName);
					}
					else
						return gcnew CodeFileContext(openFileDialog->FileName);
				}
				return nullptr;
			}

	public: static FileContext ^ createFileContext(String ^ fileName) 
			{
				if(String::Compare(Path::GetExtension(fileName), cIDEProjExt) == 0) {
					return gcnew ProjectFileContext(fileName);
				}
				else
					return gcnew CodeFileContext(fileName);
	
			}

	private: 
		literal String ^ cDialogFilters = "All IDE files|*.bproj;*.b;*.bt;*.bf|IDE Projects|*.bproj|Brainthread sourcefiles|*.bt|Brainfuck sourcefiles|*.b;*.bf|All files|*.*";
		literal String ^ cIDEProjExt = ".bproj";
	};
}