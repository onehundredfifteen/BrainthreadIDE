#pragma once

#include "OptionField.h"

namespace BrainthreadIDE
{
	using namespace System::Drawing;
	
	public ref class OptionFieldEditor : IOptionField
	{
	public:	
		OptionFieldEditor()
		{
			paired_control = nullptr;
		}

		virtual void LoadToPairedControl() {
			paired_control->richTextBox->Font = selFont;
			paired_control->richTextBox->ForeColor = selEditorForeColor;
			paired_control->richTextBox->BackColor = selEditorBackColor;
			paired_control->GutterForeColor = selMarginForeColor;
			paired_control->GutterBackColor = selMarginBackColor;
		}

		virtual void Synchronize() {
			selFont = paired_control->richTextBox->Font;
			selEditorForeColor = paired_control->richTextBox->ForeColor;
			selEditorBackColor = paired_control->richTextBox->BackColor;
			selMarginForeColor = paired_control->GutterForeColor;
			selMarginBackColor = paired_control->GutterBackColor;
		}

		virtual void Default() {
			selFont = gcnew Font("Consolas", 11);
			selEditorForeColor = System::Drawing::Color::Gray;
			selEditorBackColor = System::Drawing::Color::White;
			selMarginForeColor = System::Drawing::Color::Black;
			selMarginBackColor = System::Drawing::SystemColors::ControlLight;
		}

		virtual void PairWith(Control ^ control) {
			if(control)
				paired_control = cli::safe_cast<EditorTextBox::EditorTextBoxControl ^>(control);
		}

		virtual void SetValueFromStringArray(array<String ^> ^ val) {
			//5 lines -> font and 4 colors
			try
			{
				//like [Font: Name=Consolas, Size=11, Units=3, GdiCharSet=1, GdiVerticalFont=False]
				array<String ^> ^ font_info = val[0]->Split(gcnew array<Char>(1){','}, StringSplitOptions::RemoveEmptyEntries);
				String ^ font_name = font_info[0]->Substring(font_info[0]->IndexOf('=') +1);
				String ^ font_size = font_info[1]->Substring(font_info[1]->IndexOf('=') +1);

				selFont = gcnew Font(font_name, Convert::ToInt16(font_size));

				//like  Color [ControlLight]
				//		Color [A=255, R=54, G=201, B=193]
				selEditorForeColor = getColorFromColorString(val[1]);
				selEditorBackColor = getColorFromColorString(val[2]);
				selMarginForeColor = getColorFromColorString(val[3]);
				selMarginBackColor = getColorFromColorString(val[4]);
			}
			catch(...)
			{
				Default();
			}
		}

	property Font^ FontValue
		{
			Font^ get() { return selFont; }
		}
	property Color EditorForeColorValue
		{
			Color get() { return selEditorForeColor; }
		}
	property Color EditorBackColorValue
		{
			Color get() { return selEditorBackColor; }
		}
	property Color MarginForeColorValue
		{
			Color get() { return selMarginForeColor; }
		}
	property Color MarginBackColorValue
		{
			Color get() { return selMarginBackColor; }
		}


	private:
		EditorTextBox::EditorTextBoxControl ^ paired_control;

		Font ^ selFont;
		Color selEditorForeColor;
		Color selEditorBackColor;
		Color selMarginForeColor;
		Color selMarginBackColor;

	private:
		Color getColorFromColorString(String ^ val) {
			int pos = val->IndexOf('[');
			String ^ color_name = val->Substring(pos + 1, val->IndexOf(']') - pos - 1);

			Color c = Color::FromName(color_name);

			if(c.A == 0x0)
			{ //parse string from argb -> A=255, R=54, G=201, B=193
				array<String ^> ^ color_info = color_name->Split(gcnew array<Char>(1){','}, StringSplitOptions::RemoveEmptyEntries);

				c = System::Drawing::Color::FromArgb( 0xff, 
					Convert::ToInt16( color_info[1]->Substring(color_info[1]->IndexOf('=') +1)), 
					Convert::ToInt16( color_info[2]->Substring(color_info[2]->IndexOf('=') +1)), 
					Convert::ToInt16( color_info[3]->Substring(color_info[3]->IndexOf('=') +1)));
			}

			return c;
		}

	};
}