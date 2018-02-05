#include "StdAfx.h"
#include "SyntaxHighLighter.h"

namespace BrainthreadIDE 
{

	DefaultSyntaxHighLighter::DefaultSyntaxHighLighter(void)
	{
		CharColors = gcnew System::Collections::Generic::Dictionary<Char, System::Drawing::Color>;

		this->SetColors();
		this->SetSyntax();
	}

	void DefaultSyntaxHighLighter::HighLight(System::Windows::Forms::RichTextBox ^ richTextBox)
	{
		int i = 0, n = 0;
		System::Drawing::Color color, nextchar_color;
		
		while(i < richTextBox->TextLength)
		{
			if (CharColors->TryGetValue(richTextBox->Text[i], color))
			{
				n = 1;
				richTextBox->SelectionStart = i;

				while((i + n) < richTextBox->TextLength &&
					  CharColors->TryGetValue(richTextBox->Text[(i + n)], nextchar_color) &&
					  nextchar_color == color)
			    {
					++n;
				}
				
			    richTextBox->SelectionLength = n;
				richTextBox->SelectionColor = color;

				i += n;
			}
			else ++i;
		}

		richTextBox->SelectionLength = 0;
	}

	void DefaultSyntaxHighLighter::HighLightLastChar(System::Windows::Forms::RichTextBox ^ richTextBox)
	{
		System::Drawing::Color color;

		if(richTextBox->SelectionStart < 1)
			return;

		if (CharColors->TryGetValue(richTextBox->Text[richTextBox->SelectionStart - 1], color) == false)
		{
			color = richTextBox->ForeColor;
		}

		richTextBox->SelectionStart = richTextBox->SelectionStart - 1;
		richTextBox->SelectionLength = 1;
		richTextBox->SelectionColor = color;
		richTextBox->SelectionStart = richTextBox->SelectionStart + 1;
		richTextBox->SelectionLength = 0;
	}

	void DefaultSyntaxHighLighter::HighLightAll(System::Windows::Forms::RichTextBox ^ richTextBox)
	{
		System::Windows::Forms::RichTextBox ^ tmpRichTextBox = gcnew System::Windows::Forms::RichTextBox;

		tmpRichTextBox->Font = richTextBox->Font;
		tmpRichTextBox->ForeColor = richTextBox->ForeColor;
		tmpRichTextBox->Text = richTextBox->Text;

		this->HighLight(tmpRichTextBox);
	 
		richTextBox->Rtf = tmpRichTextBox->Rtf;
	
		delete tmpRichTextBox;
	}

	void DefaultSyntaxHighLighter::HighLightAndPaste(System::Windows::Forms::RichTextBox ^ richTextBox, String ^ Text)
	{
		System::Windows::Forms::RichTextBox ^ tmpRichTextBox = gcnew System::Windows::Forms::RichTextBox;

		tmpRichTextBox->Font = richTextBox->Font;
		tmpRichTextBox->ForeColor = richTextBox->ForeColor;
		tmpRichTextBox->Text = Text;

		this->HighLight(tmpRichTextBox);
	 
		richTextBox->SelectedRtf = tmpRichTextBox->Rtf;
	
		delete tmpRichTextBox;
	}

	List<Char> ^ DefaultSyntaxHighLighter::GetInstructionChars()
	{
		return gcnew List<Char>(CharColors->Keys);
	}

	void DefaultSyntaxHighLighter::SetSyntax()
	{
		CharColors->Clear();
	}

	///
	void BrainfuckSyntaxHighLighter::SetSyntax()
	{
		DefaultSyntaxHighLighter::SetSyntax();

		CharColors->Add('[', LoopColor);
		CharColors->Add(']', LoopColor);
		CharColors->Add('<', MovesColor);
		CharColors->Add('>', MovesColor);
		CharColors->Add('+', OperationColor);
		CharColors->Add('-', OperationColor);
		CharColors->Add('.', InputColor);
		CharColors->Add(',', InputColor);
	}

	void PBrainSyntaxHighLighter::SetSyntax()
	{
		BrainfuckSyntaxHighLighter::SetSyntax();
		
		CharColors->Add('(', OperationColor_2);
		CharColors->Add(')', OperationColor_2);
		CharColors->Add(':', OperationColor_2);
	}

	void BrainforkSyntaxHighLighter::SetSyntax()
	{
		BrainfuckSyntaxHighLighter::SetSyntax();
		
		CharColors->Add('Y', OperationColor_3);
	}

	void BrainthreadSyntaxHighLighter::SetSyntax()
	{
		BrainfuckSyntaxHighLighter::SetSyntax();
		
		CharColors->Add('(', OperationColor_2);
		CharColors->Add(')', OperationColor_2);
		CharColors->Add('*', OperationColor_2);

		CharColors->Add('&', OperationColor_4);
		CharColors->Add('^', OperationColor_4);
		CharColors->Add('~', OperationColor_4);
		CharColors->Add('%', OperationColor_4);

		CharColors->Add('{', OperationColor_3);
		CharColors->Add('}', OperationColor_3);
		CharColors->Add('!', OperationColor_3);

		CharColors->Add(':', InputColor);
		CharColors->Add(';', InputColor);
	}

	void BrainloveSyntaxHighLighter::SetSyntax()
	{
		BrainfuckSyntaxHighLighter::SetSyntax();
		
		CharColors->Add('(', LoopColor);
		CharColors->Add(')', LoopColor);
		CharColors->Add('~', OperationColor_2);
		CharColors->Add('$', OperationColor_2);
		CharColors->Add('!', OperationColor_2);
	}

	void ToadskinSyntaxHighLighter::SetSyntax()
	{
		BrainfuckSyntaxHighLighter::SetSyntax();
		
		for(Char c = 'A'; c <= 'Z'; ++c) {
			CharColors->Add(c, OperationColor_2);
		}

		CharColors->Add(';', OperationColor_3);
		CharColors->Add(':', OperationColor_3);
		CharColors->Add('%', OperationColor_3);
	}

	//Colors

	///http://alexgorbatchev.com/SyntaxHighlighter/manual/themes/emacs.html
	void DefaultSyntaxColors::SetColors(void) 
	{
		currentSyntaxHighLighter->LoopColor = System::Drawing::Color::FromArgb( 0xff, 0x33, 0x33, 0x33 ); //dark grey 333333
		currentSyntaxHighLighter->InputColor = System::Drawing::Color::FromArgb( 0xff, 0x79, 0x5d, 0xa3 ); //purple 795da3
		currentSyntaxHighLighter->MovesColor = System::Drawing::Color::FromArgb( 0xff, 0xa7, 0x1d, 0x5d ); //light violet a71d5d
		currentSyntaxHighLighter->OperationColor = System::Drawing::Color::FromArgb( 0xff, 0x0, 0x86, 0xb3 ); //turkus 0086b3

		currentSyntaxHighLighter->OperationColor_2 = System::Drawing::Color::FromArgb( 0xff, 0x45, 0xa1, 0x63 ); //light green 45a163;
		currentSyntaxHighLighter->OperationColor_3 = System::Drawing::Color::FromArgb( 0xff, 0xf4, 0x80, 0x24 ); //orange f48024;
		currentSyntaxHighLighter->OperationColor_4 = System::Drawing::Color::FromArgb( 0xff, 0x10, 0x87, 0xdc ); //steel blue 1087dc;
		//currentSyntaxHighLighter->SharedOperationColor_4 = System::Drawing::Color::FromArgb( 0xff, 0x0, 0x95, 0xff ); //darker steel blue #0095ff;
	}

	void LightSyntaxColors::SetColors(void) 
	{
		currentSyntaxHighLighter->LoopColor = System::Drawing::Color::FromArgb( 0xff, 0x2a, 0x0, 0xff ); //blue 2a00ff
		currentSyntaxHighLighter->InputColor = System::Drawing::Color::FromArgb( 0xff, 0x33, 0x33, 0x33 ); //dark grey 333333
		currentSyntaxHighLighter->MovesColor = System::Drawing::Color::FromArgb( 0xff, 0x0, 0x99, 0x0 ); //green 009900 
		currentSyntaxHighLighter->OperationColor = System::Drawing::Color::FromArgb( 0xff, 0xec, 0x0f, 0x0f ); //kinda red ec0f0f

		currentSyntaxHighLighter->OperationColor_2 = System::Drawing::Color::FromArgb( 0xff, 0xf4, 0x80, 0x24 ); //orange f48024;
		currentSyntaxHighLighter->OperationColor_3 = System::Drawing::Color::FromArgb( 0xff, 0x8c, 0x1c, 0xde ); //orange 8c1cde;
		currentSyntaxHighLighter->OperationColor_4 = System::Drawing::Color::FromArgb( 0xff, 0xaa, 0x77, 0x0 ); //brown aa7700;
		//currentSyntaxHighLighter->SharedOperationColor_4 = System::Drawing::Color::FromArgb( 0xff, 0x7f, 0x0, 0x55 ); //purple 7f0055;
	}

	void DarkSyntaxColors::SetColors(void) 
	{
		currentSyntaxHighLighter->LoopColor = System::Drawing::Color::FromArgb( 0xff, 0xd3, 0xd3, 0xd3 ); //smokewhite d3d3d3
		currentSyntaxHighLighter->InputColor = System::Drawing::Color::FromArgb( 0xff, 0x00, 0xff, 0xff ); //aqua 00ffff
		currentSyntaxHighLighter->MovesColor = System::Drawing::Color::FromArgb( 0xff, 0xff, 0xaa, 0x3e ); //orange ffaa3e
		currentSyntaxHighLighter->OperationColor = System::Drawing::Color::FromArgb( 0xff, 0x81, 0xce, 0xf9 ); //teal 81cef9

		currentSyntaxHighLighter->OperationColor_2 = System::Drawing::Color::FromArgb( 0xff, 0x5c, 0xe6, 0x38 ); //grass green 5ce638;
		currentSyntaxHighLighter->OperationColor_3 = System::Drawing::Color::FromArgb( 0xff, 0xe3, 0xe6, 0x58 ); //green yellow e3e658;
		//currentSyntaxHighLighter->OperationColor_4 = System::Drawing::Color::FromArgb( 0xff, 0xff, 0x9e, 0x7b ); //coral ff9e7b;
		currentSyntaxHighLighter->OperationColor_4 = System::Drawing::Color::FromArgb( 0xff, 0xee, 0xdd, 0x10 ); //golden eedd10;
	}

	void RandomSyntaxColors::SetColors(void) 
	{
		Random ^ random_seed = gcnew Random();
		System::Drawing::Color color = this->getRandomColor(random_seed);
		//random 4 colors
		currentSyntaxHighLighter->LoopColor = this->getRandomColor(random_seed); 
		currentSyntaxHighLighter->MovesColor = color;
		currentSyntaxHighLighter->InputColor = color;
		currentSyntaxHighLighter->OperationColor = this->getRandomColor(random_seed);

		//same as functions color
		color = this->getRandomColor(random_seed);
		currentSyntaxHighLighter->OperationColor_2 = color; 
		currentSyntaxHighLighter->OperationColor_3 = color;
		currentSyntaxHighLighter->OperationColor_4 = color;
	}

		/*
		richTextBox->SelectionLength = 3;
				richTextBox->SelectionBackColor = System::Drawing::Color::Purple;


	MemoryStream ^ DefaultSyntaxHighLighter::HighlightSyntax(String ^ str, System::Drawing::Color default_color)
	{
		System::Drawing::Color color, nextchar_color;
		int i = 0, n = 0;
		array<System::Drawing::Color> ^ colors = gcnew array<System::Drawing::Color>;
		MemoryStream ^ memoryStream = gcnew MemoryStream();
        StreamWriter ^ writer = new StreamWriter(memoryStream);

		CharColors->Values->CopyTo(colors, 0); 

		writer->Write("{\\rtf1\\ansi\\ansicpg1250\\deff0\\deflang1045{\\fonttbl{\\f0\\fnil\\fcharset238 Consolas;}}\n");
		writer->Write("{\c\olortbl ;\\red");
		writer->Write(default_color.R);
		writer->Write("\\green");
		writer->Write(default_color.G);
		writer->Write("\\blue");
		writer->Write(default_color.B);
		writer->Write(";");

		for each(System::Drawing::Color c in colors)
		{
			writer->Write("\\red");
			writer->Write(c.R);
			writer->Write("\\green");
			writer->Write(c.G);
			writer->Write("\\blue");
			writer->Write(c.B);
			writer->Write(";");
		}
		writer->Write("}\n\\viewkind4\\uc1\\pard\\cf1\\f0\\fs23 ");
		
		while(i < str->Length)
		{
			if (CharColors->TryGetValue(str[i], color) == false)
				color = default_color;


			{
				n = 1;
				richTextBox->SelectionStart = i;

				while((i + n) < str->Length &&
					  (CharColors->TryGetValue(strText[(i + n)], nextchar_color) || ) &&
					  nextchar_color == color)
			    {
					++n;
				}
				
			    richTextBox->SelectionLength = n;
				richTextBox->SelectionColor = color;

				writer->Write("\\cf " + str.SubString(i, n));

				i += n;
			}
			else ++i;
		}

		richTextBox->SelectionLength = 0;


{\rtf1\ansi\ansicpg1250\deff0\deflang1045{\fonttbl{\f0\fnil\fcharset238 Consolas;}}
{\colortbl ;\red128\green128\blue128;\red0\green134\blue179;\red121\green93\blue163;\red51\green51\blue51;\red69\green161\blue99;}
\viewkind4\uc1\pard\cf1\f0\fs23 test now 10 \cf2 ++++++++++\cf3 :\cf1  \cf4 [\cf2 -\cf4 ]\cf5 *\cf1  \par
test now 11 \cf2 +++++++++++\cf3 :\cf1  \cf4 [\cf2 -\cf4 ]\cf5 *\cf1  \par
test now 12 \cf2 ++++++++++++\cf3 :\cf1  \cf4 [\cf2 -\cf4 ]\cf5 *\cf1  \par
test now 13 \cf2 +++++++++++++\cf3 :\cf1  \cf4 [\cf2 -\cf4 ]\cf5 *\cf1  \par
test now 14 \cf2 ++++++++++++++\cf3 :\cf1  \cf4 [\cf2 -\cf4 ]\cf5 *\cf1  \par
}
 
	}*/
	
}
