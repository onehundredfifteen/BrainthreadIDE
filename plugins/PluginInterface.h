#pragma once

namespace BrainthreadIDE
{
    using namespace System;	

	public interface class IBtPlugin
	{
		String ^ Name();
		String ^ Description();

		//return null if not case
		String ^ OnInvoke(String ^ code, Language language);
		String ^ OnPragma(String ^ code, Language language);
	};
}