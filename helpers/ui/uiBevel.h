#pragma once

namespace BrainthreadIDE
{
    using namespace System;	
	using namespace System::Windows::Forms;

	public ref class uiBevel
	{
	public:	
		uiBevel(Control ^ control,  System::Drawing::Rectangle _rect) : rect(_rect)
		{
			control->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &uiBevel::bevelPaint); 
		}

		uiBevel(Control ^ control, int _padding) : padding(_padding)
		{
			control->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &uiBevel::bevelPaintFill); 
		}

		uiBevel(int _padding) : padding(_padding)
		{
		}

	protected:
		System::Drawing::Rectangle rect;
		int padding;
	
	private: System::Void bevelPaint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 
			ControlPaint::DrawBorder3D( e->Graphics, rect, Border3DStyle::RaisedInner );
		}
	private: System::Void bevelPaintFill(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 
			System::Drawing::Rectangle borderRectangle =  cli::safe_cast<Control^ >(sender)->ClientRectangle;
			borderRectangle.Inflate( -padding, -padding );
			ControlPaint::DrawBorder3D( e->Graphics, borderRectangle, Border3DStyle::RaisedInner );
		}

	};

	public ref class uiHorizontallyAlignedBevel : uiBevel
	{
	public:	
		uiHorizontallyAlignedBevel(Control ^ control, int _padding, int _y, int _height) : uiBevel(_padding), y(_y), height(_height)
		{
			control->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &uiHorizontallyAlignedBevel::bevelPaint); 
		}
	private:
		 int y;
		 int height;
	
	private: System::Void bevelPaint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 
				 System::Drawing::Rectangle borderRectangle =  cli::safe_cast<Control^ >(sender)->ClientRectangle;
				 borderRectangle.Inflate( -padding, -0 );

				 borderRectangle.Y = y;
				 borderRectangle.Height = height;

				 ControlPaint::DrawBorder3D( e->Graphics, borderRectangle, Border3DStyle::RaisedInner );
			 }

	};

	public ref class uiHorizontalLineBevel : uiHorizontallyAlignedBevel
	{
	public:	
		uiHorizontalLineBevel(Control ^ control, int _padding, int _y) : uiHorizontallyAlignedBevel(control, _padding, _y, 2)
		{
		}

	};
}