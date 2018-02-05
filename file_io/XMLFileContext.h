#pragma once

#include "FileContext.h"

namespace BrainthreadIDE 
{
	using namespace System::Xml;

	public ref class XMLFileContext : public FileContext
	{
	public:
		XMLFileContext(String ^ filepath) : FileContext(filepath)
		{
			xmldoc = gcnew XmlDocument;
		}

	protected:
		XmlDocument^ xmldoc;

	public:
		property XmlDocument ^ Content {
			XmlDocument ^ get() {
				return xmldoc;
			}
		}

	public:
		virtual bool Open() override
		{
			StreamReader ^ streamReader;
		  
			try
			{
				streamReader = gcnew StreamReader(this->filePath, System::Text::Encoding::Default, true);
				this->xmldoc->Load(streamReader);
			}
			catch(XmlException ^ xmlex)
			{
				MessageBox::Show( String::Format("XML is malformed. Additional info: {0}", xmlex->Message), "Load error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
			catch(Exception ^)
			{
				return false;
			}
		  
			streamReader->Close();

		    return true;
		}

		virtual bool Save() override
		{
			StreamWriter ^ streamWriter;

			try
			{
				streamWriter = gcnew StreamWriter(this->filePath);
				this->xmldoc->Save(streamWriter);
			}
			catch(XmlException ^ xmlex)
			{
				MessageBox::Show( String::Format("XML is malformed. Additional info: {0}", xmlex->Message), "Save error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
			catch(Exception ^ ex)
			{
				MessageBox::Show( String::Format("Cannot save to file. Reason: {0}", ex->Message), "Save error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}

			  streamWriter->Close();

			  return true;
		}

	protected:
		XmlElement ^ GetNodeByName(String ^ name)
		{
			  for each(XmlElement ^ node in this->xmldoc->DocumentElement->ChildNodes)
			  {
					if(node->Name == name ){
						return node;
					}
			  }
		  
			  return nullptr;
		}

	};
}

