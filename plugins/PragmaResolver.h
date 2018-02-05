#pragma once

#include "PluginManager.h"
#include "../WorkContexts.h"

namespace BrainthreadIDE
{
    using namespace System;	

	public ref class PragmaResolver
	{
	public:	
		PragmaResolver(String ^ c, PluginManager ^ pm) 
		{
			this->code = c;
			this->pluginManager = pm;

			this->nextPragmaSearchPos = 0;
			this->pluginNames = this->pluginManager->PragmaPluginNames();
		}

		bool HasPragmas()
		{
			return (this->nextPragma() != nullptr) && this->pluginNames->Count > 0;
		}

		void Resolve(WorkContext ^ workContext, Language lang)
		{
			String ^ resolved_code;
			Plugin ^ currentPlugin;
			String ^ pluginName;

			int ioa, pragma_start;
			
			this->nextPragmaSearchPos = 0;
		    String ^ pragma = this->nextPragma();

			workContext->outputLister->AddOutputWithTimestamp("Resolving pragmas...");

			while(pragma)
			{
				ioa = pragma->IndexOfAny(gcnew array<wchar_t>(3){' ','\n','\t'});
				pluginName = pragma->Substring(0, ioa)->Trim();
				currentPlugin = this->pluginManager->GetPlugin( pluginName );
					
				if(currentPlugin && this->pluginNames->IndexOf( currentPlugin->Name() ) >= 0) //pragma plugin installed
				{
					pragma_start = this->nextPragmaSearchPos - pragma->Length - cPragmaOp->Length;
					workContext->outputLister->AddIDEOutput(String::Format("Resolving pragma \"{0}\" at position {1}", pluginName, pragma_start));
					
					resolved_code = currentPlugin->OnPragma(pragma->Substring(ioa)->Trim(), lang);
					this->code = this->code->Remove(pragma_start, pragma->Length);
					this->code = this->code->Insert(pragma_start, resolved_code);

					this->nextPragmaSearchPos += (resolved_code->Length - pragma->Length);
				}
				else if(currentPlugin) {
					workContext->outputLister->AddIDEOutput(String::Format("Cannot resolve a pragma \"{0}\" because this plugin doesn't support pragmas", pluginName));
				}
				else {
					workContext->outputLister->AddIDEOutput(String::Format("Cannot resolve a pragma \"{0}\" because this plugin is neither installed nor allowed to execute", pluginName));
				}

				pragma = this->nextPragma();
			}
			
			if(String::Compare(this->code, workContext->editorTextBox->richTextBox->Text))
			{
				workContext->editorTextBox->richTextBox->Text = this->code;
				workContext->syntaxHighLighter->HighLightAll(workContext->editorTextBox->richTextBox);
			}
		}

	private:
		String ^ nextPragma()
		{
			String ^ pragma;
			int pragma_end, pragma_start;
			int search_end = this->code->Length - this->nextPragmaSearchPos - cPragmaOp->Length;

			if(search_end >= 0) {
				pragma_start = this->code->IndexOf(cPragmaOp, this->nextPragmaSearchPos, search_end ); // -3 for safe futher search
			
				if(pragma_start >= 0) {
					pragma_end = this->code->IndexOf(cPragmaOp, pragma_start + cPragmaOp->Length, this->code->Length - (pragma_start + cPragmaOp->Length));

					if(pragma_end >= 0) {
						pragma = this->code->Substring(pragma_start + cPragmaOp->Length, pragma_end - pragma_start - cPragmaOp->Length);
						this->nextPragmaSearchPos = pragma_end + cPragmaOp->Length; 

						//pragma must be minimum 5 chars long and plugin name 3
						if(pragma->Length >= 5 && pragma->IndexOfAny( gcnew array<wchar_t>(3){' ','\n','\t'}) >= 3 )
							return pragma;
						else
							return nextPragma();
					}
				}
			}

			return nullptr;
		}

	private:
		String ^ code;
		PluginManager ^ pluginManager;

		List<String ^> ^ pluginNames;
		int nextPragmaSearchPos;

	private:
		literal String ^ cPragmaOp = "@@@";
	};
}