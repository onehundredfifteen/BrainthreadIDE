#pragma once

#include "FileContext.h"

namespace BrainthreadIDE 
{
	using namespace System::Text;

	public ref class ProtectedFileContext : public FileContext
	{
	public:
		ProtectedFileContext(String ^ filepath) : FileContext(filepath)
		{
			this->sourceContext = nullptr;
		}
		//as Decorator
		ProtectedFileContext(String ^ filepath, FileContext ^ sourceContext) : FileContext(filepath)
		{
			this->sourceContext = sourceContext;
			this->Open();
		}
	protected:
		FileContext ^ sourceContext;

	public:
		virtual bool Open() override
		{
			if(this->sourceContext)
				this->Content = this->sourceContext->Content;
			else if(false == FileContext::Open())
				return false;
			else
				this->Content = this->decode64(this->Content);

		    return true;
		}

		virtual bool Save() override
		{
			String ^ plaintext = this->fileContent;
			//do not set modified flag [this->Content]
			this->fileContent = this->encode64(this->fileContent);
			
			bool res = FileContext::Save();

			this->fileContent = plaintext;

			return res;
		}

	protected:
		String ^ encode64(String ^ toEncode)
		{
		  Encoding^ ascii = Encoding::ASCII;
		  array<Byte> ^inArray = ascii->GetBytes( toEncode );
		 
		  return Convert::ToBase64String( inArray, 0, inArray->Length, Base64FormattingOptions::InsertLineBreaks );
		}
		String ^ decode64(String ^ toDecode)
		{
		  Encoding^ ascii = Encoding::ASCII;
		  array<Byte> ^outArray = Convert::FromBase64String( toDecode );
		  
		  return ascii->GetString( outArray );
		}

	};
}

