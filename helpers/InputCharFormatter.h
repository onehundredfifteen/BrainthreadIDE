#pragma once

namespace BrainthreadIDE 
{
	using namespace System::Text;
	using namespace System::Text::RegularExpressions;

	/*
	Gets a string with char codes to replace in.
	Input: "{98}+{115}={enter}9"
	Produces: "b+s=
			  
	*/

	public ref class InputCharFormatter 
	{
	public: static String ^ Format(String ^ s) 
			{
				String ^ pattern = "{eof}|{end}|{nl}|{enter}|" + Regex::Escape("{") + "([0-9]{1,3})}";
				Regex^ regex = gcnew Regex(pattern);

				MatchEvaluator^ evaluator = gcnew MatchEvaluator( &InputCharFormatter::Replace );

				return regex->Replace(s, evaluator);
			}
	private: static String^ Replace(Match ^ match)
		    {
				int num;
				Encoding^ ascii = Encoding::ASCII;
				String ^ capture = Regex::Replace(match->Value, "{|}", String::Empty);
			
				try
				{
					num = Convert::ToInt32(capture);
				}
				catch(FormatException ^)
				{
					if(capture->ToLower() == "eof" || capture->ToLower() == "end")
						num = 0;
					else if(capture->ToLower() == "nl" || capture->ToLower() == "enter")
						num = 10;
					else return "";
				}
				catch(...){
					return "";
				}

			    return ascii->GetString( gcnew array<unsigned char>{num} );
		    }
	};
}