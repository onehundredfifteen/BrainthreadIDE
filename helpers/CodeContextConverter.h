#pragma once

#include "../WorkContexts.h"

namespace BrainthreadIDE 
{
	using namespace System::Text;
	//using namespace System::Windows::Forms;

	public ref class CodeContextConverter 
	{
	public: CodeContextConverter(WorkContext ^ ctx) : syntaxHighLighter(ctx->syntaxHighLighter), 
													  editorText(ctx->editorTextBox->richTextBox->Text) 
			{
			}
	//TODO get source, get raw source as strings
	/*public: int JumpOneInstruction(int cursor)
			{
				if(cursor >= editorText->Length) 
					return cursor;
				else if(editorText[cursor] != '[')
					return cursor + 1;

				return CodeLoopBinder::GetPairedInstruction(cursor, editorText);
			}*/

	public: int ToCursor(int instruction)
			{
				int n = 0, i = instruction;
				List<Char> ^ instructions = syntaxHighLighter->GetInstructionChars();

				for each(Char c in editorText)
				{
					if(instructions->Contains(c))
					{
						if(i > 0)
							--i;
						else 
							break;
					}

					++n;
				}

				return n;;
			}
	public: int ToInstruction(int cursor)
			{
				int n = 0;
				List<Char> ^ instructions = syntaxHighLighter->GetInstructionChars();

				for(int i = 0; i < cursor; ++i)
				{
					if(instructions->Contains(editorText[i]))
						++n;
				}

				return n;
			}

	public: property String ^ RawSource
			{
				String ^ get() 
				{
					List<Char> ^ instructions = syntaxHighLighter->GetInstructionChars();
					StringBuilder ^ sb = gcnew StringBuilder();

					for each(Char c in editorText)
					{
						if(instructions->Contains(c))
							sb->Append((System::Char)(c));
					}

					return sb->ToString();
				}
			}
	

	private:
		DefaultSyntaxHighLighter ^ syntaxHighLighter;
		String ^ editorText;
	};
}