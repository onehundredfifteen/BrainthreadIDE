#pragma once

namespace BrainthreadIDE 
{
	using namespace System::Text;

	public ref class CodeLoopBinder 
	{
	private: 
		static Dictionary<char, char> ^ instructionPairs;

	public: 
		static CodeLoopBinder()
		{
			instructionPairs = gcnew Dictionary<char, char>(); 

			instructionPairs->Add('[',']');
			instructionPairs->Add(']','[');
			instructionPairs->Add('(',')');
			instructionPairs->Add(')','(');
			instructionPairs->Add('{','}');
			instructionPairs->Add('}','{');
			instructionPairs->Add('<','>');
			instructionPairs->Add('>','<');
		}

	public: 
		static int FindEndOfLoop(int pos, String ^ code) 
		{
			if(pos >= code->Length || code[pos] != '[')
				return pos;

			return GetPairedInstruction(pos, code);
		}

		static int GetPairedInstruction(int pos, String ^ code) 
		{
			int i, p_cnt = 1;
			char c, c_pair;

			if(pos >= code->Length || instructionPairs->ContainsKey( (char)code[pos]) == false)
				return pos;

			c = (char)code[pos];
			c_pair = (char)instructionPairs[c];

			if(c < c_pair) //look at right [ -> ]
			{
				for(i = pos + 1; p_cnt && i < code->Length; ++i)
				{
					if(code[i] == c)
						++p_cnt;
					else if(code[i] == c_pair)
						--p_cnt;
				}

				return i - 1;
			}
			else //look at left ] -> [
			{
				for(i = pos - 1; p_cnt && i >= 0; --i)
				{
					if(code[i] == c)
						++p_cnt;
					else if(code[i] == c_pair)
						--p_cnt;
				}

				return i + 1;
			}
		}
	
	};
}