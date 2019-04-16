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
			int result = pos;
			char c, c_pair;

			if(pos >= code->Length || instructionPairs->ContainsKey( (char)code[pos]) == false)
				return result;

			c = (char)code[pos];
			c_pair = (char)instructionPairs[c];
			
			//look at right [ -> ]
			if((c < c_pair && c != '<') || c == '>') // > is rather paired with < at right
			{ 
				result = lookRight(pos, c, c_pair, code);
				if(result == -1)//not found? invert direction
					return lookLeft(pos, c, c_pair, code);
				else
					return result;
			}
			else //look at left ] -> [
			{
				result = lookLeft(pos, c, c_pair, code);
				if(result == -1)
					return lookRight(pos, c, c_pair, code);
				else
					return result;
			}
		}

	private: 
		static int lookRight(int start_pos, char c,  char c_pair, String ^ code)
		{
			int i, p_cnt = 1;

			for(i = start_pos + 1; p_cnt && i < code->Length; ++i)
			{
				if(code[i] == c)
					++p_cnt;
				else if(code[i] == c_pair)
					--p_cnt;
			}

			return p_cnt ? -1 : (i - 1);
		}

		static int lookLeft(int start_pos, char c,  char c_pair, String ^ code)
		{
			int i, p_cnt = 1;

			for(i = start_pos - 1; p_cnt && i >= 0; --i)
			{
				if(code[i] == c)
					++p_cnt;
				else if(code[i] == c_pair)
					--p_cnt;
			}

			return p_cnt ? -1 : (i + 1);
		}
	
	};
}