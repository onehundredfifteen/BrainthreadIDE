#include "StdAfx.h"
#include "GlobalOptions.h"

namespace BrainthreadIDE 
{
	void GlobalOptions::PairControl(FieldId field_id, System::Windows::Forms::Control ^ opControl)
	{
		field_list[field_id]->PairWith(opControl);
	}
	
	void GlobalOptions::PairControls(array<System::Windows::Forms::Control ^> ^ opWindowControls)
	{
		int i = 0;

		for each(IOptionField ^ fields in field_list->Values)
		{
			fields->PairWith(opWindowControls[i++]);
		}
	}

	void GlobalOptions::Apply(void)
	{
		for each(IOptionField ^ fields in field_list->Values)
		{
			fields->Synchronize();
		}

		//apply plugins
		pluginManager->GrantPrivileges( safe_cast<OptionFieldPlugin ^>(field_list[FieldId::fiPlugins])->Value );
	}
	void GlobalOptions::Default(void)
	{
		for each(IOptionField ^ fields in field_list->Values)
		{
			fields->Default();
		}
	}

	void GlobalOptions::Load(void)
	{
		for each(IOptionField ^ fields in field_list->Values)
		{
			fields->LoadToPairedControl();
		}
	}

	//other functions
	void GlobalOptions::RecentFilesListAdd(String ^ newFileName)
	{				
		if(false == recentFilesList->Contains(newFileName) && System::IO::File::Exists(newFileName))
		{
			recentFilesList->Enqueue( newFileName );

			if(recentFilesList->Count > maxrecentFilesListCount)
				recentFilesList->Dequeue();
		}
	}

	//file functions
	void GlobalOptions::LoadFromFile(void)
	{
		const int interpreter_op_len = 20;
		const int editor_index = 24;
		const int editor_op_len = 5;
		const int plugin_cnt_index = 36;
		const int min_valid_ops_cnt = 38;

		Int32 read_plugin_cnt, read_recent_cnt;
		int recent_cnt_index = 0;

		array<String ^> ^ options_plugins = nullptr;
		array<String ^> ^ options_interpreter = gcnew array<String ^>(20);
		array<String ^> ^ options_editor = gcnew array<String ^>(5);
		//read options line by line
		array<String ^> ^ options = optionsFile->Content->Split(gcnew array<Char>(1){'\n'}, StringSplitOptions::RemoveEmptyEntries);

		if(options->Length < min_valid_ops_cnt || Int32::TryParse(options[ plugin_cnt_index ], read_plugin_cnt) == false){
			return;
		}

		Array::ConstrainedCopy(options, 0, options_interpreter, 0, interpreter_op_len);
		Array::ConstrainedCopy(options, editor_index, options_editor, 0, editor_op_len);
		
		safe_cast<OptionFieldInterpreter ^>(field_list[FieldId::fiInterpreters])->SetValueFromStringArray(options_interpreter); //5 items -> 6 to 10

		safe_cast<OptionField<bool> ^>(field_list[FieldId::fiReadLogFlag])->SetValueFromString( options[ interpreter_op_len + 0]); 
		safe_cast<OptionField<String ^> ^>(field_list[FieldId::fiLogPath])->SetValueFromString( options[ interpreter_op_len + 1]); 
		safe_cast<OptionField<bool> ^>(field_list[FieldId::fiSaveCodeBeforeRun])->SetValueFromString( options[ interpreter_op_len + 2]); 
		safe_cast<OptionField<bool> ^>(field_list[FieldId::fiPauseProgramAfterRun])->SetValueFromString( options[ interpreter_op_len + 3]); 

		safe_cast<OptionFieldEditor ^>(field_list[FieldId::fiEditor])->SetValueFromStringArray(options_editor); //5 items -> 6 to 10

		safe_cast<OptionField<int>  ^>(field_list[FieldId::fiSyntaxStyle])->SetValueFromString(options[ editor_index + editor_op_len + 0]); 
		safe_cast<OptionField<bool> ^>(field_list[FieldId::fiTraceNewThread])->SetValueFromString( options[ editor_index + editor_op_len + 1]); 
		safe_cast<OptionField<bool> ^>(field_list[FieldId::fiMemViewOnThreadCaptionClick])->SetValueFromString( options[ editor_index + editor_op_len + 2]); 
		safe_cast<OptionField<int>  ^>(field_list[FieldId::fiThreadNames])->SetValueFromString( options[ editor_index + editor_op_len + 3]); 
		safe_cast<OptionField<bool> ^>(field_list[FieldId::fiStepSelect])->SetValueFromString( options[ editor_index + editor_op_len + 4]);
		safe_cast<OptionField<int>  ^>(field_list[FieldId::fiDebugeeWindowStyle])->SetValueFromString( options[ editor_index + editor_op_len + 5]); 
		safe_cast<OptionField<int>  ^>(field_list[FieldId::fiDebugeeWindowPosition])->SetValueFromString( options[ editor_index + editor_op_len + 6]);


		//read plugins
		if(read_plugin_cnt > 0)
		{
			array<String ^> ^ options_plugins = gcnew array<String ^>(read_plugin_cnt);
			Array::ConstrainedCopy(options, plugin_cnt_index + 1, options_plugins, 0, read_plugin_cnt);
			safe_cast<OptionFieldPlugin ^>(field_list[FieldId::fiPlugins])->SetValueFromStringArray(options_plugins);

			this->pluginManager->GrantPrivileges( safe_cast<OptionFieldPlugin ^>(field_list[FieldId::fiPlugins])->Value );
		}

		//read recent files
		recent_cnt_index = plugin_cnt_index + read_plugin_cnt + 1; //calculate index
		if(Int32::TryParse(options[ recent_cnt_index ], read_recent_cnt) == false){
			return;
		}
			
		for(int i = 0; i < read_recent_cnt ; i++)
		{
			this->RecentFilesListAdd( options[ recent_cnt_index + i + 1] );
		}
		
	}

	void GlobalOptions::SaveToFile(void)
	{
		System::Text::StringBuilder ^ sb = gcnew System::Text::StringBuilder();

		//interpreter info [10 slots]
		for(int i = 0; i < 10; ++i)
		{
			if(i >= (int)Language::l__spare || String::IsNullOrEmpty(this->InterpreterPath[ (BrainthreadIDE::Language)i ]))
			{
				sb->Append("*\n*\n");
				continue;
			}
			
			sb->Append(this->InterpreterPath[ (BrainthreadIDE::Language)i ]);
		    sb->Append("\n");
			sb->Append(this->InterpreterArgs[ (BrainthreadIDE::Language)i ]);
		    sb->Append("\n");
		}

		sb->Append(this->ReadLogFlag);
		sb->Append("\n");
		sb->Append(this->LogPath);
		sb->Append("\n");
		sb->Append(this->SaveCodeBeforeRun);
		sb->Append("\n");
		sb->Append(this->PauseProgramAfterRun);
		sb->Append("\n");
		sb->Append(this->EditorFont);
		sb->Append("\n");
		sb->Append(this->EditorForeColor);
		sb->Append("\n");
		sb->Append(this->EditorBackColor);
		sb->Append("\n");
		sb->Append(this->MarginForeColor);
		sb->Append("\n");
		sb->Append(this->MarginBackColor);
		sb->Append("\n");
		sb->Append(this->SyntaxStyle);
		sb->Append("\n");
		sb->Append(this->TraceNewThread);
		sb->Append("\n");
		sb->Append(this->MemViewOnThreadCaptionClick);
		sb->Append("\n");
		sb->Append(this->ThreadNaming);
		sb->Append("\n");
		sb->Append(this->DebugStepSelect);
		sb->Append("\n");
		sb->Append(this->DebugeeWindowStyle);
		sb->Append("\n");
		sb->Append(this->DebugeeWindowPosition);
		sb->Append("\n");

		//plugin granted
		List<String ^> ^ granted_plugins = safe_cast<OptionFieldPlugin ^>(field_list[FieldId::fiPlugins])->Value;
		sb->Append(granted_plugins->Count);
		sb->Append("\n");
		for each(String ^ gp in granted_plugins)
		{
			sb->Append(gp);
		    sb->Append("\n");
		}
		
		//recent files list
		sb->Append(this->RecentFilesList->Length);
		sb->Append("\n");
		for each(String ^ rfile in this->RecentFilesList)
		{
			sb->Append(rfile);
		    sb->Append("\n");
		}

		//finally save
		optionsFile->Content = sb->ToString();
		optionsFile->Save();
	}
	
}


