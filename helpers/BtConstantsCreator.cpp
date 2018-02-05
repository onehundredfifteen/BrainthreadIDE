#include "StdAfx.h"
#include "BtConstantsCreator.h"

namespace BrainthreadIDE 
{	
	String ^ BtConstantsCreator::GetNumberConstant(const int n) 
	{
		String ^ s1 = GetRawNumber(n), ^s2, ^s3;

		if(n < 15){
			return s1;
		}
		else
		{
			if(n % 2)
			{
				s2 = GetRawNumber(n - 1);
				s3 = GetRawNumber(n + 1);
		
				if( (s1->Length <= s2->Length || (n < cSmallDivLim && s1->IndexOf('[') < s2->IndexOf('['))) &&
				    (s1->Length <= s3->Length || (n < cSmallDivLim && s1->IndexOf('[') < s3->IndexOf('['))) )
					return s1;

				return (s2->Length <= s3->Length) ? s2 + "+" : s3 + "-";
			}
			else
			{
				s2 = GetRawNumber(n - 1);
				s3 = GetRawNumber(n - 2);
				
				if( (s2->Length + 1 <= s1->Length || (n < cSmallDivLim && s2->IndexOf('[') < s1->IndexOf('['))) &&
				    (s2->Length + 1 <= s3->Length || (n < cSmallDivLim && s2->IndexOf('[') < s3->IndexOf('['))) )
					  return s2 + "+";

				return (s1->Length <= s3->Length + 2) ? s1 : s3 + "++";
			}
		}
	}

	String ^ BtConstantsCreator::GetRawNumber(const int n) 
	{
		String ^ str_nc;
		StringBuilder ^ sb = gcnew StringBuilder();
		array<int> ^ factors = GetNumberFactors(n);

		if(factors == nullptr){
			return "[-]";
		}
		else if(factors->Length == 1){
			return gcnew String('+', factors[0]);
		}
		else if(factors->Length < 4) 
		{
			int pair_mod = 0;

			if(factors[ factors->Length - 1 ] >= 15 && n >= 103 && Array::IndexOf(factors, 1) < 0) 
			{
				Array::Sort(factors);
				Array::Reverse(factors);

				str_nc = GetNumberConstant(factors[0]);
				pair_mod = str_nc->LastIndexOf('>') > str_nc->LastIndexOf('<') ? 1 : 0;

				sb->Append(str_nc);
			}
			else
				sb->Append(gcnew String('+', factors[ 0 ]) );

			for (int f = 1; f < factors->Length; ++f)
			{
				if((f + pair_mod) % 2) 
					sb->Append("[>" + gcnew String('+', factors[ f ]) + "<-]>");
				else
					sb->Append("[<" + gcnew String('+', factors[ f ]) + ">-]<");
			}
			
			return sb->ToString();
		}
		else //power series [1000+]
		{
			//++++[>++++++++++<-]>+++++[<++++++++++>-]<+++[>++++++++++<-]>[<++++++++++>-]<++++++
             // 4   *10            +5)   *10            +3) *10            +0)*10         +6
			
			sb->Append( gcnew String('+', factors[ 0 ]) );

			for (int f = 1; f < factors->Length; ++f)
			{
				if(f % 2) 
					sb->Append("[>++++++++++<-]>");
				else
					sb->Append("[<++++++++++>-]<");

				if(factors[ f ] > 0){
					sb->Append(gcnew String('+', factors[ f ]));
				}
			}

			return sb->ToString();
		}
	}

	array<int> ^ BtConstantsCreator::GetNumberFactors(const int n) 
	{
		if(n == 0){ 
			return nullptr;
		}
		else if(n < 15){ //simple numbers
			return gcnew array<int>{n};
		}
		else if(n < 103){//small numbers - shortest sum of n divisors
			Tuple<int, int> ^ divisors = FindBestDivisors(GetDivisors(n)->ToArray(), n);

			return gcnew array<int>{ divisors->Item2, divisors->Item1 };
		}
		else if(n < 1000){//not too big numbers - try to get from a*b -> (x*y)*b
			Tuple<int, int> ^ divisors = FindBest3Divisors(GetDivisors(n)->ToArray(), n);
		
			int dmax = Math::Max(divisors->Item1, divisors->Item2);
			Tuple<int, int> ^ divisors_dmax = FindBestDivisors(GetDivisors(dmax)->ToArray(), dmax);

			if(dmax >= 15 && dmax < n && divisors_dmax->Item1 > 2 && divisors_dmax->Item2 > 2) {
				return gcnew array<int>{ divisors_dmax->Item2, divisors_dmax->Item1, Math::Min(divisors->Item1, divisors->Item2) };
			}
			else {
				return gcnew array<int>{ divisors->Item2, divisors->Item1 };
			}	
		}
		else //power series
		{
			int q, m = n;
			List<int> ^ factor_series = gcnew List<int>;
			for(q = 3; Math::Pow(10, q) <= n; ++q);

			while(--q >= 0)
			{
			   factor_series->Add( m / Math::Pow(10, q) ); 
			 
			   m = m % static_cast<int>(Math::Pow(10, q));
			}

			return factor_series->ToArray();
		}
	}
	
	Tuple<int, int> ^ BtConstantsCreator::FindBestDivisors(array<int> ^ all_divisors, const int n) 
	{
		Tuple<int, int> ^ best_pair = gcnew Tuple<int,int>(1, n);
		for each(int divisor_a in all_divisors)
		{
			for each(int divisor_b in all_divisors)
			{
				if(divisor_a * divisor_b == n &&
				   divisor_a + divisor_b < best_pair->Item1 + best_pair->Item2)
				{
					best_pair = gcnew Tuple<int,int>(divisor_a, divisor_b);
				}
			}
		}
		return best_pair;
	}

	Tuple<int,int> ^ BtConstantsCreator::FindBest3Divisors(array<int> ^ all_divisors, const int n) 
	{
		Tuple<int,int> ^ best_pair = gcnew Tuple<int,int>(1, n);
		for each(int divisor_a in all_divisors)
		{
			for each(int divisor_b in all_divisors)
			{
				if(divisor_a * divisor_b == n && 
					   divisor_a + divisor_b < best_pair->Item1 + best_pair->Item2 &&
					   ((n < 180 && divisor_a > 14) || (n < 3000 && divisor_a > 16))
					   )
				{
					best_pair = gcnew Tuple<int,int>(divisor_a, divisor_b);
				}
			}
		}

		return best_pair;
	}
	
	List<int> ^ BtConstantsCreator::GetDivisors(const int n) 
	{
		int lim = n / 2; //(n % 2) ? static_cast<int>(Math::Floor( Math::Sqrt(n) )) : n/2;
		List<int> ^ divisors = gcnew List<int>();

		for(int i = 2; i <= lim; i++) {
			if(n % i == 0)
				divisors->Add(n / i);
			}

		return divisors;
	}
	/*
	Form ^ BtConstantsCreator::CreateDialog()
	{
		Form ^ prompt = gcnew Form();
				
		prompt->Width = 300;
		prompt->Height = 160;
		prompt->FormBorderStyle = FormBorderStyle::FixedDialog;
		prompt->Text = "Write a number or character";
		prompt->StartPosition = FormStartPosition::CenterScreen;
		prompt->MaximizeBox = false;      
		prompt->MinimizeBox = false;

				Panel ^ panel = gcnew Panel;
				panel->Dock = System::Windows::Forms::DockStyle::Fill;
				panel->Paint += gcnew PaintEventHandler(  &BtConstantsCreator::panelPaint );
				
				
				Label ^ labelDescription = gcnew Label;
				labelDescription->Text = "Write yr";
				labelDescription->Location = Point(2*cMargin, 2*cMargin);
				labelDescription->AutoSize = true;

				TextBox ^ textboxConstant = gcnew TextBox;
				textboxConstant->Multiline = true;
				//textboxConstant->Text = GetNumberConstant(21);
				//textboxConstant->Text = GetNumberConstant(37);
				textboxConstant->Text = GetNumberConstant(69);
				textboxConstant->Location = Point(2*cMargin, 4*cMargin);
				textboxConstant->Size = System::Drawing::Size(240, 22);

				Button ^ buttonConfirmation = gcnew Button;
				buttonConfirmation->Text = "Insert";
				buttonConfirmation->Location = Point(2*cMargin, 6*cMargin);

				Button ^ buttonCancel = gcnew Button;
				buttonCancel->Text = "Cancel";
				buttonCancel->Location = Point(140, 6*cMargin);

		//end
		prompt->AcceptButton = buttonConfirmation;
		prompt->CancelButton = buttonCancel;

		//add
		prompt->Controls->Add( panel );
		panel->Controls->Add( labelDescription );
		panel->Controls->Add( textboxConstant );
		panel->Controls->Add( buttonConfirmation );
		panel->Controls->Add( buttonCancel );

		return prompt;
	}
	void BtConstantsCreator::panelPaint (Object^ sender, PaintEventArgs^ e)
	{
		Rectangle borderRectangle =  cli::safe_cast<Panel^ >(sender)->ClientRectangle;
		borderRectangle.Inflate( -cMargin, -cMargin );
		ControlPaint::DrawBorder3D( e->Graphics, borderRectangle, Border3DStyle::RaisedInner );
	}

	*/
}