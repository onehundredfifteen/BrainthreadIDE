#pragma once

#include "../file_io/ProtectedFileContext.h"

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Text;
	using namespace System::Collections::Generic;

	public ref class BtOutputGenerator
	{
	public:
		static BtOutputGenerator() {
			
			Template = gcnew array<String ^, 2>(256, 256);

			ProtectedFileContext ^ memFile = gcnew ProtectedFileContext(String::Format("{0}\\res\\psgarr.dat", FileContext::BaseDirectory() ));

			if(memFile->Open())
				InitFromMemory(memFile);
			else {
				Init();
				SaveToMemory(memFile);
			}
		}

		static String ^ Generate(String ^ s);
	
	private:
		static void Init();
		static void InitFromMemory(FileContext ^ memFile);
		static void SaveToMemory(FileContext ^ memFile);

	private:
		static array<String ^, 2> ^ Template;          
		static const int tp_len = 256;

	};
}

