#pragma once

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Text;
	using namespace System::Collections::Generic;

	public ref class BtConstantsCreator
	{
		public: static String ^ GetNumberConstant(const int n); 
		public: static String ^ GetRawNumber(const int n);
		//private: static String ^ GetNumberString(const int n);
		private: static array<int> ^ GetNumberFactors(const int n); 
		private: static Tuple<int, int> ^ FindBestDivisors(array<int> ^ all_divisors, const int n);
		private: static Tuple<int, int> ^ FindBest3Divisors(array<int> ^ all_divisors, const int n);
		private: static List<int> ^ GetDivisors(const int n);
			
		private: static const int cMargin = 13; 
		private: static const int cSmallDivLim = 34;

	};
}