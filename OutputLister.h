#pragma once
namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Collections::Generic;

	public ref class OutputLister
	{
	public:
		OutputLister(ListView ^ list, TextBox ^ textbox) : outputList(list), outputBox(textbox)
		{
			outputStrings = gcnew List<String ^>; 
			outputItems = gcnew Dictionary<ListViewItem ^, OutputLister::OutputItem ^>; 
		}

		void Refresh();
		void Purge();

		void AddOutput(System::Collections::Generic::List<String ^> ^);
		void AddOutput(String ^ line);
		void AddIDEOutput(String^ line);
		void AddOutputWithTimestamp(String ^ line);

		int GetErrorPosition(ListViewItem ^ listItem);

	private:
		ref class OutputItem
	    {
		public:	
			String ^ description;
			String ^ kind;
			int instruction;
			bool important;
		};

    private:
		ListView ^ outputList;
		TextBox ^ outputBox;
		List<String ^> ^ outputStrings;
		Dictionary<ListViewItem ^, OutputLister::OutputItem ^> ^ outputItems;

		//TabPage ^ activePage;
	
	private:
		OutputItem ^ ParseOutput(String ^ line);
		int GetInstructionFromLine(String ^ line);

	private: 
		literal String ^ cMessage = "Message";
		literal String ^ cError = "Error";
		literal String ^ cWarning = "Warning";
		literal String ^ cCodeValid = "Code is valid";
		literal String ^ cCodeSane = "Code is sane";
		literal String ^ cCodeHas = "Code has";
		literal String ^ cInstructionAt = "at instruction";
		literal String ^ cTooManyWarnings = "Too many warning or error messages";
	};
}

