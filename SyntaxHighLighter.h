#pragma once
namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Collections::Generic;

	public ref class DefaultSyntaxHighLighter
	{
	public:
		DefaultSyntaxHighLighter(void);

		void HighLightAll(System::Windows::Forms::RichTextBox ^ richTextBox);
		void HighLightLastChar(System::Windows::Forms::RichTextBox ^ richTextBox);
		void HighLightAndPaste(System::Windows::Forms::RichTextBox ^ richTextBox, String ^ Text);
		virtual void SetSyntax(void);
		virtual void SetColors(void){};

		List<Char> ^ GetInstructionChars();

	internal:
		System::Drawing::Color LoopColor;
		System::Drawing::Color MovesColor;
		System::Drawing::Color InputColor;
		System::Drawing::Color OperationColor;
		System::Drawing::Color OperationColor_2; //colors of operationtypes
		System::Drawing::Color OperationColor_3;
		System::Drawing::Color OperationColor_4;

	private:
		void HighLight(System::Windows::Forms::RichTextBox ^ richTextBox);

	internal:
		Dictionary<Char, System::Drawing::Color>^ CharColors;
	};

	//BrainfuckSyntaxHighLighter
	public ref class BrainfuckSyntaxHighLighter : public DefaultSyntaxHighLighter
	{
	public:
		BrainfuckSyntaxHighLighter(void) : DefaultSyntaxHighLighter(){};

		virtual void SetSyntax(void) override;
	};

	//PBrainSyntaxHighLighter
	public ref class PBrainSyntaxHighLighter : public BrainfuckSyntaxHighLighter
	{
	public:
		PBrainSyntaxHighLighter(void) : BrainfuckSyntaxHighLighter(){};

		virtual void SetSyntax(void) override;
	};

	//BrainthreadSyntaxHighLighter
	public ref class BrainthreadSyntaxHighLighter : public BrainfuckSyntaxHighLighter
	{
	public:
		BrainthreadSyntaxHighLighter(void) : BrainfuckSyntaxHighLighter(){};

		virtual void SetSyntax(void) override;
	};

	//BrainforkSyntaxHighLighter
	public ref class BrainforkSyntaxHighLighter : public BrainfuckSyntaxHighLighter
	{
	public:
		BrainforkSyntaxHighLighter(void) : BrainfuckSyntaxHighLighter(){};

		virtual void SetSyntax(void) override;
	};

	//BrainloveSyntaxHighLighter
	public ref class BrainloveSyntaxHighLighter : public BrainfuckSyntaxHighLighter
	{
	public:
		BrainloveSyntaxHighLighter(void) : BrainfuckSyntaxHighLighter(){};

		virtual void SetSyntax(void) override;
	};

	//ToadskinSyntaxHighLighter
	public ref class ToadskinSyntaxHighLighter : public BrainfuckSyntaxHighLighter
	{
	public:
		ToadskinSyntaxHighLighter(void) : BrainfuckSyntaxHighLighter(){};

		virtual void SetSyntax(void) override;
	};


	//DECORATORS

	//colorizers [decorator pattern]
	public ref class DefaultSyntaxColors : public DefaultSyntaxHighLighter
	{
	public:
		DefaultSyntaxColors(DefaultSyntaxHighLighter ^sh) : currentSyntaxHighLighter(sh), DefaultSyntaxHighLighter(){}

		virtual void SetColors(void) override;
		virtual void SetSyntax(void) override
		{ 
			currentSyntaxHighLighter->SetSyntax(); 
			this->CharColors = currentSyntaxHighLighter->CharColors;
		} 

	protected:
		DefaultSyntaxHighLighter ^ currentSyntaxHighLighter;
	};

	public ref class LightSyntaxColors : public DefaultSyntaxColors
	{
	public:
		LightSyntaxColors(DefaultSyntaxHighLighter ^sh) : DefaultSyntaxColors(sh){}

		virtual void SetColors(void) override;
	};

	public ref class DarkSyntaxColors : public DefaultSyntaxColors
	{
	public:
		DarkSyntaxColors(DefaultSyntaxHighLighter ^sh) : DefaultSyntaxColors(sh){}

		virtual void SetColors(void) override;
	};

	public ref class RandomSyntaxColors : public DefaultSyntaxColors
	{
	public:
		RandomSyntaxColors(DefaultSyntaxHighLighter ^sh, bool _dark_bg) : DefaultSyntaxColors(sh), dark_background(_dark_bg){}

		virtual void SetColors(void) override;

	private:
		bool dark_background;

    private:
		System::Drawing::Color getRandomColor(Random ^ random_seed) {
			int brightness, r, g, b;

			do //random color with background color style adjustment
			{
				r = random_seed->Next(0,256);
				g = random_seed->Next(0,256);
				b = random_seed->Next(0,256);
				brightness = (r * 299 + g * 587 + b * 114) / 1000;
			}				 
			while((brightness < 110 && this->dark_background) || (brightness >= 110 && this->dark_background == false));

			return System::Drawing::Color::FromArgb(0xff, r, g, b);
		}

	};

}

