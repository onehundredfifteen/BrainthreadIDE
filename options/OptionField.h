 #pragma once

namespace BrainthreadIDE
{
    using namespace System;	
	using namespace System::Windows::Forms;

	public interface class IOptionField
	{
		void LoadToPairedControl();
		void Synchronize();
		void Default();

		void PairWith(Control ^ control);
    };

	template < typename E >
	public ref class OptionField : IOptionField
	{
	public:	
		OptionField(E def_value)
		{
			paired_control = nullptr;
			default_value = def_value;
			saved_value = default_value;
		}

		virtual void Synchronize();
		virtual void LoadToPairedControl();
		
		virtual void SetValueFromString(String ^ val);

		virtual void Default() {
			 saved_value = default_value;
		}

		virtual void PairWith(Control ^ control) {
			if(control)
				paired_control = control;
		}

	property E Value
		{
			E get() { return this->saved_value; }
		}

	protected:
		E saved_value;
		E default_value;
		Control ^ paired_control;
	};
}