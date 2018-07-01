#pragma once

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Text;
	using namespace System::Collections::Generic;

	public ref class CandidateComparer : System::Collections::IComparer
	{
	public:
	   virtual int Compare(Object^ x, Object^ y) = System::Collections::IComparer::Compare
	   {
		  String ^ _x = dynamic_cast<String ^>(x);
		  String ^ _y = dynamic_cast<String ^>(y);
		  
		  return _x->Length - _y->Length;
	   }
	};

	public ref class BtConstantsGenerator
	{
		public: 
			static String ^ Generate(const int n); 
			static String ^ GetNumberConstantReversed(const int n);
			static bool HasMultipleLoops(String ^ s);
			
		private: 
			static String ^ GetRawNumber(const int n);
			static array<int> ^ GetNumberFactors(const int n); 
			static Tuple<int, int> ^ FindBestDivisors(array<int> ^ all_divisors, const int n);
			static Tuple<int, int> ^ FindBest3Divisors(array<int> ^ all_divisors, const int n);
			static List<int> ^ GetDivisors(const int n);

		private: 
			static const int cMargin = 13; 
			static const int cSimpleNumLim = 15;
			static const int cSmallDivLim = 34;

	};
}