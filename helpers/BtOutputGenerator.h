#pragma once

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Text;
	using namespace System::Collections::Generic;

	public ref class BtOutputGenerator
	{
	public:
		static BtOutputGenerator() {
			
			G = gcnew array<String ^, 2>(256, 256);
			Init();
		}

		static String ^ Generate(String ^ s);
	
	private:
		static void Init();

	private:
		static array<String ^, 2> ^ G;          
	

	};
}

