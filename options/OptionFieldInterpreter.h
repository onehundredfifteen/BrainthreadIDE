#pragma once

#include "Language.h"
#include "OptionField.h"

namespace BrainthreadIDE
{
	using namespace System::Collections::Generic;
	
	public ref class OptionFieldInterpreter : IOptionField
	{
	public:	
		OptionFieldInterpreter(String ^ brainhtread_def_path, List<String ^> ^ args)
		{
			paired_control = nullptr;

			selInterpreterInfo = gcnew List< Tuple<String ^, String ^> ^>;
			selBrainhtreadDefPath = brainhtread_def_path;

			selDefArgs = args;
		}

		virtual void LoadToPairedControl() {
			int i = 0;
			array<Object ^> ^ rowVals;
			
			paired_control->Rows->Clear();

			for each(Tuple<String ^, String ^> ^ tu in selInterpreterInfo)
			{
				rowVals = gcnew array<Object ^>(2){tu->Item1,tu->Item2};

				if((BrainthreadIDE::Language)i >= Language::l__spare)
					continue;

				paired_control->Rows->Add(rowVals);
				paired_control->Rows[i]->HeaderCell->Value = LanguageName::GetLanguageName((BrainthreadIDE::Language) i);
				++i;
			}	
		}

		virtual void Synchronize() {
			
			selInterpreterInfo->Clear();

			for each(DataGridViewRow ^ row in paired_control->Rows) 
			{
				selInterpreterInfo->Add( gcnew Tuple<String ^, String ^>( dynamic_cast<String^>(row->Cells[0]->Value), 
																		  dynamic_cast<String^>(row->Cells[1]->Value)));
			}
		}

		virtual void Default() {

			selInterpreterInfo->Clear();

			for each(String ^ arg in this->selDefArgs)
			{
				if(String::IsNullOrEmpty(arg))
					selInterpreterInfo->Add( gcnew Tuple<String ^, String ^>("", arg));
				else
					selInterpreterInfo->Add( gcnew Tuple<String ^, String ^>(this->selBrainhtreadDefPath, arg));
			}
		}

		virtual void PairWith(Control ^ control) {
			if(control)
				paired_control = dynamic_cast<DataGridView ^>(control);
		}

		virtual void SetValueFromStringArray(array<String ^> ^ val) {

			selInterpreterInfo->Clear();
			for (int i = 0; i < val->Length; i += 2) 
			{
				if(val[i] != "*")
					selInterpreterInfo->Add( gcnew Tuple<String ^, String ^>(val[i], val[i+1]) );
				else
					selInterpreterInfo->Add( gcnew Tuple<String ^, String ^>("", "") );
			}
		}
		
	property List< Tuple<String ^, String ^> ^> ^ Value
		{
			List< Tuple<String ^, String ^> ^> ^ get() { return selInterpreterInfo; }
		}

	private:
		System::Windows::Forms::DataGridView ^ paired_control;

		List< Tuple<String ^, String ^> ^> ^ selInterpreterInfo;
		
		String ^ selBrainhtreadDefPath;
		List<String ^> ^ selDefArgs;

	};
}