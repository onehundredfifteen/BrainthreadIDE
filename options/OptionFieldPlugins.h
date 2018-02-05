#pragma once

#include "OptionField.h"

namespace BrainthreadIDE
{
	using namespace System::Collections::Generic;
	
	public ref class OptionFieldPlugin : IOptionField
	{
	public:	
		OptionFieldPlugin(List<String ^> ^ plugin_names)
		{
			paired_control = nullptr;

			selAllPlugins = plugin_names;
			selSelectedPlugins = gcnew List<String ^>();
		}

		virtual void LoadToPairedControl() {
			int i = 0;
			paired_control->Items->Clear();

			for each(String ^ plugin in selAllPlugins)
			{
				paired_control->Items->Add(plugin);
				paired_control->SetItemChecked(i++, selSelectedPlugins->IndexOf(plugin) >= 0);
			}
		}

		virtual void Synchronize() {
			System::Collections::IEnumerator ^ etor = paired_control->CheckedItems->GetEnumerator();
			
			selSelectedPlugins->Clear();

			while(etor->MoveNext()) {
				selSelectedPlugins->Add(etor->Current->ToString());
			}
		}

		virtual void Default() {
			selSelectedPlugins->Clear();
		}

		virtual void PairWith(Control ^ control) {
			if(control)
				paired_control = cli::safe_cast<CheckedListBox ^>(control);
		}

		virtual void SetValueFromStringArray(array<String ^> ^ val) {
			for each(String ^ allowed_plugin in val)
				selSelectedPlugins->Add(allowed_plugin);
		}

	property List<String ^> ^ Value
		{
			List<String ^> ^ get() { return selSelectedPlugins; }
		}

	private:
		CheckedListBox ^ paired_control;

		List<String ^> ^ selAllPlugins;
		List<String ^> ^ selSelectedPlugins;


	};
}