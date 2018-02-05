#pragma once

#include "CodeFileContext.h"
#include "ProjectSettingsFileContext.h"
#include "../options/PageSettings.h"

namespace BrainthreadIDE 
{
	public ref class ProjectFileContext : public CodeFileContext
	{
	public:
		ProjectFileContext(void)
		{
			optionsFile = gcnew ProjectSettingsFileContext("");
		}
		ProjectFileContext(String ^ filepath) : CodeFileContext(filepath)
		{
			optionsFile = gcnew ProjectSettingsFileContext(filepath);
		}
		/*
		explicit static operator ProjectFileContext (FileContext^ fc)
		{
			//ProjectFileContext p(fs->)
			return ProjectFileContext();
		}*/

		PageSettings ^ settingsContent;

		virtual property String ^ Title {
			String ^ get() override {
				return String::Format("Project \"{0}\"", fileName);
			}
		}

		virtual bool Open() override
		{
			  if(optionsFile->Open() == false)
				  return false;

			  this->settingsContent = gcnew PageSettings;
			  this->filePath = optionsFile->LinkedFilePath;
			  this->fileName = Path::GetFileName(optionsFile->LinkedFilePath);

			  this->settingsContent->ReadOptions(optionsFile->Content);
		  
			  if(false == FileContext::Open())
				  return false;

			  //save first version
			  this->backupContent = this->fileContent;

			  return true;
		}

		virtual bool Save() override
		{
			  if(this->HasPhysicalFile() == false)
				  return this->SaveAs();  

			  optionsFile->LinkedFilePath = this->filePath;
			  this->settingsContent->WriteOptions(optionsFile->Content);

			  return FileContext::Save() && optionsFile->Save();
		}

		virtual bool SaveAs() override
		{
			  if(optionsFile->GetSaveAsPath() == false)
				  return false;

			  this->filePath = Path::ChangeExtension(optionsFile->FilePath, ".bt");
			  this->fileName = Path::GetFileName(this->filePath);

			  return ProjectFileContext::Save();
		}

		void Restore()
		{
			  //restore first version
			  this->fileContent = this->backupContent;
		}

	private:
		ProjectSettingsFileContext ^ optionsFile;
		
		String ^ backupContent;
	};
}

