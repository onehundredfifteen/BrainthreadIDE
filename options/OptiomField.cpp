#include "StdAfx.h"
#include "OptionField.h"

namespace BrainthreadIDE 
{
	template < typename E >
	void OptionField<E>::Synchronize()
	{
		//saved_value = paired_control->Text.Trim();
	}

	template <>
	void OptionField<String ^>::Synchronize()
	{
		saved_value = paired_control->Text->Trim();
	}

	template <>
	void OptionField<bool>::Synchronize()
	{
		saved_value = cli::safe_cast<CheckBox ^>(paired_control)->Checked;
	}

	template <>
	void OptionField<int>::Synchronize()
	{
		saved_value = cli::safe_cast<ComboBox ^>(paired_control)->SelectedIndex;
	}

	//LoadToPairedControl
	template < typename E >
	void OptionField<E>::LoadToPairedControl()
	{
		 paired_control->Text = cli::safe_cast<Object ^(saved_value)->ToString();
	}

	template <>
	void OptionField<String ^>::LoadToPairedControl()
	{
		 paired_control->Text = saved_value;
	}

	template <>
	void OptionField<bool>::LoadToPairedControl()
	{
		 cli::safe_cast<CheckBox ^>(paired_control)->Checked = saved_value;
	}

	template <>
	void OptionField<int>::LoadToPairedControl()
	{
		 cli::safe_cast<ComboBox ^>(paired_control)->SelectedIndex = saved_value;
	}

	//SetValueFromString
	template < typename E >
	void OptionField<E>::SetValueFromString(String ^ val)
	{
		 //paired_control->Text = cli::safe_cast<Object ^(saved_value)->ToString();
	}

	template <>
	void OptionField<String ^>::SetValueFromString(String ^ val)
	{
		 saved_value = val;
	}

	template <>
	void OptionField<bool>::SetValueFromString(String ^ val)
	{
		 saved_value = val->ToLower() == "true" ? true : false;
	}

	template <>
	void OptionField<int>::SetValueFromString(String ^ val)
	{
		 saved_value = Convert::ToInt16(val);
	}
}