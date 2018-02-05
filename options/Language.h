#pragma once

namespace BrainthreadIDE 
{
	public enum class Language : int
	{
			lBrainthread = 0,
			lBrainfuck = 1,
			lPBrain = 2,
			lBrainfork = 3,
			lBrainlove = 4,
			lToadskin = 5,
			l__spare = 6
	};

	public ref class LanguageName
	{
	public: static System::String ^ GetLanguageName(BrainthreadIDE::Language lang) {	
			switch(lang)
			{
				case Language::lBrainthread: return "Brainthread"; 
				case Language::lBrainfuck: return "Brainfuck";
				case Language::lPBrain: return "pBrain";
				case Language::lBrainfork: return "Brainfork";
				case Language::lBrainlove: return "Brainlove";
				case Language::lToadskin: return "Toadskin";
				
				default: return "Unknown lang"; 
			}
		}
	};
}

