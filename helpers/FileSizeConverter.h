#pragma once

namespace BrainthreadIDE 
{
	using namespace System::Text;

	public ref class FileSizeConverter 
	{
	public: static String ^ GetSize(int size) 
			{
				String ^ s;
				
				if(size < 1) {
					return "";
				}
				else if(size < 1024) {
					return size + " B";
				}
				else if(size < 1048576) //1024 * 1024
				{
					s = float(size / 1024.0).ToString();
					return s->Substring(0, Math::Min(s->Length, 5)) + " kB";
				}
				else
				{
					s = float(size / 1048576.0).ToString();
					return s->Substring(0, Math::Min(s->Length, 5)) + " MB";
				}
			}
	
	};
}