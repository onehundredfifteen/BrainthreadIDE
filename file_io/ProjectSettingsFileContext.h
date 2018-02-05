#pragma once

#include "XMLFileContext.h"

namespace BrainthreadIDE 
{
	public ref class ProjectSettingsFileContext : public XMLFileContext
	{
	public:
		ProjectSettingsFileContext(String ^ filepath) : XMLFileContext(filepath)
		{	
			XmlElement ^ nodeRoot;
		
			XmlDeclaration ^ xmldecl = xmldoc->CreateXmlDeclaration("1.0", String::Empty, String::Empty); // Create declaration
			xmldoc->AppendChild(xmldecl);
		
			nodeRoot = xmldoc->CreateElement(cxmlnodeProject); // Create main node
			nodeRoot->SetAttribute(cxmlnodeattrVersion, cxmlnodeattrVersionValue);
			xmldoc->AppendChild(nodeRoot);
			 
			nodeRoot->AppendChild(xmldoc->CreateElement(cxmlnodePath)); //Create sourcepath
		}

	private:
		String ^ linkedFilePath;

	public:
		property String ^ LinkedFilePath {
			String ^ get() {
				return linkedFilePath;
			}
			void set(String ^ val) {
				linkedFilePath = val;
			}
		}
		
		virtual bool Open() override
		{
			  if(XMLFileContext::Open() == false)
				  return false;

			  XmlElement ^ nodePath = this->GetNodeByName(cxmlnodePath);
			  if(nodePath)
			  {
					this->linkedFilePath = nodePath->InnerText;
					return true;
			  }
		  
			  return false;
		}

		virtual bool Save() override
		{
			  XmlElement ^ nodePath = this->GetNodeByName(cxmlnodePath);
			  if(nodePath)
			  {
					nodePath->InnerText = this->linkedFilePath;
					return XMLFileContext::Save();
			  }

			  return false;
		}

		virtual bool GetSaveAsPath() //override
		{
			  SaveFileDialog ^ saveFileDialog = gcnew SaveFileDialog();

			  saveFileDialog->Filter = cDialogFilters;
			  saveFileDialog->FilterIndex = 1;
			  saveFileDialog->Title = "Save project as";
			  saveFileDialog->RestoreDirectory = true;

			  if(this->filePath == String::Empty)
				 saveFileDialog->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
			  else
				 saveFileDialog->InitialDirectory = Path::GetDirectoryName(this->filePath);

			  if (saveFileDialog->ShowDialog() == DialogResult::OK)
			  {
					this->filePath = saveFileDialog->FileName;
					this->fileName = Path::GetFileName(saveFileDialog->FileName);

					return true;
			  }

			  return false;
		}

	private: 
		literal String ^ fileExtension = ".bproj";
		literal String ^ cDialogFilters = "IDE Projects|*.bproj";

		//xml nodes
		literal String ^ cxmlnodeProject = "btide_project";
		literal String ^ cxmlnodePath = "sourcefilepath";

		//xml attributes
		literal String ^ cxmlnodeattrVersion = "version";

		//xml project file version value
		literal String ^ cxmlnodeattrVersionValue = "1.0";

	
	};
}

