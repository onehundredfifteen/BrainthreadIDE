#pragma once

#include "../SyntaxHighLighter.h"
#include "../options/GlobalOptions.h"	

namespace BrainthreadIDE 
{
	public ref class SyntaxHighLighterFactory 
	{
	public: 
		static DefaultSyntaxHighLighter ^ createSyntaxHighLighter(Language lang) 
		{
			switch(GlobalOptions::Instance->SyntaxStyle)
			{
				case 0:
					return gcnew DefaultSyntaxColors( getLanguageSyntaxHighLighter(lang));
				case 1:
					return gcnew LightSyntaxColors( getLanguageSyntaxHighLighter(lang));
				case 2:
					return gcnew DarkSyntaxColors( getLanguageSyntaxHighLighter(lang));
				case 3:
					return gcnew RandomSyntaxColors( getLanguageSyntaxHighLighter(lang), false);
				default:
					return getLanguageSyntaxHighLighter(lang);
					
			}
		}

		static DefaultSyntaxHighLighter ^ createSyntaxHighLighter(DefaultSyntaxHighLighter ^ sh) 
		{
			switch(GlobalOptions::Instance->SyntaxStyle)
			{
				case 0:
					return gcnew DefaultSyntaxColors(sh);
				case 1:
					return gcnew LightSyntaxColors(sh);
				case 2:
					return gcnew DarkSyntaxColors(sh);
				case 3:
					return gcnew RandomSyntaxColors(sh, false);
				default:
					return sh;
			}
		}

	private: 
		static DefaultSyntaxHighLighter ^ getLanguageSyntaxHighLighter(Language lang) 
		{
			switch(lang)
			{
				case Language::lBrainthread:
					return gcnew BrainthreadSyntaxHighLighter;
				case Language::lBrainfuck:
					return gcnew BrainfuckSyntaxHighLighter;
				case Language::lPBrain:
					return gcnew PBrainSyntaxHighLighter;
				case Language::lBrainfork:
					return gcnew BrainforkSyntaxHighLighter;
				case Language::lBrainlove:
					return gcnew BrainloveSyntaxHighLighter;
				case Language::lToadskin:
					return gcnew ToadskinSyntaxHighLighter;
				default:
					return gcnew BrainfuckSyntaxHighLighter;
			}
		}
	};
}