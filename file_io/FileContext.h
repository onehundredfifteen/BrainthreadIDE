#pragma once

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::IO;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;

	public ref class FileContext
	{
	protected: 
		static int fileCounter = 0;

	public:
		FileContext(void)
		{
			++fileCounter;
			isModified = false;

			fileName = String::Concat(L"untitled_", (fileCounter).ToString());
		}

		FileContext(String ^ filepath)
		{
			FileContext();

			this->filePath = filepath;
			this->fileName = Path::GetFileName(filepath);
		}
		
		~FileContext(void)
		{
			--fileCounter;
		}

	    virtual property String ^ Title {
			String ^ get() {
				return fileName;
			}
		}

		property String ^ Content {
			String ^ get() {
				return fileContent;
			}
			void set(String ^ val) {
				isModified = (String::Compare(fileContent, val) != 0);
				fileContent = val;
			}
		}

		property String ^ FilePath {
			String ^ get() {
				return filePath;
			}
		}	

		property bool IsModified {
			bool get() {
				return isModified;
			}
		}

	public: static String ^ BaseDirectory()
		{
			return Application::StartupPath; //System::Environment::CurrentDirectory;
		}
	public: static String ^ GetDirectory(String ^ filePath)
		{
			return Path::GetDirectoryName(filePath);
		}

	public:
		virtual bool Open()
		{
			  StreamReader ^ streamReader;
		  
			  try
			  {
				  streamReader = gcnew StreamReader(this->filePath, System::Text::Encoding::Default, true);
			  }
			  catch(Exception ^)
			  {
				  return false;
			  }
		  
			  this->fileContent = streamReader->ReadToEnd();
			  streamReader->Close();

			  return true;
		}

		virtual bool FileContext::Save()
		{
			  StreamWriter ^ streamWriter;

			  if(this->isModified == false)
				  return true;

			  try
			  {
				  streamWriter = gcnew StreamWriter(this->filePath);
			  }
			  catch(Exception ^ ex)
			  {
				  MessageBox::Show( String::Format("Cannot save to file. Reason: {0}", ex->Message), "Save error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				  return false;
			  }

			  streamWriter->Write(this->fileContent);
			  streamWriter->Flush();
			  streamWriter->Close();

			  this->isModified = false;

			  return true;
		}

		virtual bool FileContext::Delete()
		{
			  try
			  {
				  System::IO::File::Delete(this->filePath);
			  }
			  catch(...)
			  {
				  return false;
			  }

			  return true;
		}

		bool FileContext::HasPhysicalFile()
		{
			return this->filePath != nullptr && System::IO::File::Exists(this->filePath);
		}

	protected:
		bool isModified;
		String ^ fileContent;
		String ^ filePath;
		String ^ fileName;
	};
}

