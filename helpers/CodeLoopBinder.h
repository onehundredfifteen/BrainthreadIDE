#pragma once

namespace BrainthreadIDE 
{
	using namespace System::Text;

	public ref class CodeLoopBinder 
	{
	public: static int GetPairedInstruction(int pos, String ^ code) 
			{
				int i, p_cnt = 1;

				if(code[pos] != '[')
					return pos;

				for(i = pos + 1; p_cnt && i < code->Length; ++i)
				{
					if(code[i] == '[')
						++p_cnt;
					else if(code[i] == ']')
						--p_cnt;
				}

				return i - 1;
			}
	
	};
}