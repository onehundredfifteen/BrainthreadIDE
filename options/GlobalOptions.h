#pragma once

#include "OptionField.h"
#include "OptionFieldEditor.h"
#include "OptionFieldPlugins.h"
#include "OptionFieldInterpreter.h"

#include "../file_io/FileContext.h"
#include "../plugins/PragmaResolver.h"

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Collections::Generic;


	public ref class GlobalOptions
	{
		 enum class FieldId
			{
				fiEditor,
				fiSyntaxStyle,
				fiInterpreters,
				fiSaveCodeBeforeRun,
				fiPauseProgramAfterRun,
				fiNoPurgeOutputLog,
				fiLogPath,
				fiReadLogFlag,
				fiTraceNewThread,
				fiMemViewOnThreadCaptionClick,
				fiDebugeeWindowStyle,
				fiDebugeeWindowPosition,
				fiPlugins
			};

	    protected:
			 GlobalOptions()
			 {
			    maxrecentFilesListCount = 10;

				optionsFile = gcnew FileContext(String::Format("{0}\\ide.settings", FileContext::BaseDirectory() ));
				pluginManager = gcnew PluginManager();

				//file history
				recentFilesList = gcnew System::Collections::Generic::Queue<String ^>(maxrecentFilesListCount);

				//field defaults
				field_list = gcnew System::Collections::Generic::Dictionary< FieldId, IOptionField ^ >;

				field_list->Add(FieldId::fiEditor, gcnew OptionFieldEditor);
				field_list->Add(FieldId::fiSyntaxStyle, gcnew OptionField<int>(0));

				field_list->Add(FieldId::fiInterpreters, gcnew OptionFieldInterpreter( String::Format("{0}\\bf\\Brainthread.exe", FileContext::BaseDirectory() ), 
																						gcnew List<String ^>(gcnew array<String ^>(6){"-l bt","-l b","-l pb","-l bf","",""}) )
																					 );

				field_list->Add(FieldId::fiSaveCodeBeforeRun, gcnew OptionField<bool>(true));
				field_list->Add(FieldId::fiPauseProgramAfterRun, gcnew OptionField<bool>(false));
				field_list->Add(FieldId::fiNoPurgeOutputLog, gcnew OptionField<bool>(true));
				field_list->Add(FieldId::fiLogPath, gcnew OptionField<String ^>( FileContext::BaseDirectory() + "\\bf\\log.txt"));
				field_list->Add(FieldId::fiReadLogFlag, gcnew OptionField<bool>(false));
				
				field_list->Add(FieldId::fiTraceNewThread, gcnew OptionField<bool>(true));
				field_list->Add(FieldId::fiMemViewOnThreadCaptionClick, gcnew OptionField<bool>(true));
				field_list->Add(FieldId::fiDebugeeWindowStyle, gcnew OptionField<int>(0));
				field_list->Add(FieldId::fiDebugeeWindowPosition, gcnew OptionField<int>(0));

				field_list->Add(FieldId::fiPlugins, gcnew OptionFieldPlugin(pluginManager->PluginNames()));
				
				//set default options
				this->Default();

				//try load from file
				if(optionsFile->Open() == true)
					this->LoadFromFile();  
			 }

		     GlobalOptions(const GlobalOptions%) { throw gcnew System::InvalidOperationException("GlobalOptions singleton cannot be copy-constructed"); }
		  
        public:
			static property GlobalOptions^ Instance
			{
				GlobalOptions^ get() { return m_Instance; }
			}
			
	    private:
			static GlobalOptions^ m_Instance = gcnew GlobalOptions;

			Dictionary< FieldId, IOptionField ^ > ^ field_list;
			List<String^> ^ pluginList;
			System::Collections::Generic::Queue<String^> ^ recentFilesList;
			int maxrecentFilesListCount;

			PluginManager ^ pluginManager;
			FileContext ^ optionsFile;
	    
	    public:
			void PairControl(FieldId field_id, System::Windows::Forms::Control ^ opControl);
			void PairControls(array<System::Windows::Forms::Control ^> ^ opWindowControls);

		public:
			void Apply(void);
			void Load(void);
			void Default(void);

			void SaveToFile(void);
			void LoadFromFile(void);

			//file history add
			void RecentFilesListAdd(String ^ newFileName);

		public:
			property PluginManager ^ Plugins
			{
				PluginManager ^ get() { 
					return this->pluginManager; 
				}
			}

			//interpreter tab properties
			property bool CustomInterpreterFlag[BrainthreadIDE::Language]
			{
				bool get(BrainthreadIDE::Language lang) { 
					return false == this->isGenuineInterpreterSet(lang); 
				}
			}
			property String ^ InterpreterPath[BrainthreadIDE::Language]
			{
				String ^ get(BrainthreadIDE::Language lang) { 
					return safe_cast<OptionFieldInterpreter ^>(field_list[FieldId::fiInterpreters])->Value[(int)lang]->Item1;
				}
			}
			property String ^ InterpreterArgs[BrainthreadIDE::Language]
			{
				String ^ get(BrainthreadIDE::Language lang) { 
					return safe_cast<OptionFieldInterpreter ^>(field_list[FieldId::fiInterpreters])->Value[(int)lang]->Item2;
				}
			}
			property bool ReadLogFlag
			{
				bool get() { 
					return safe_cast<OptionField<bool> ^>(field_list[FieldId::fiReadLogFlag])->Value; 
				}
			}
			property String ^ LogPath
			{
				String ^ get() { 
					return safe_cast<OptionField<String ^> ^>(field_list[FieldId::fiLogPath])->Value; 
				}
			}
			property bool SaveCodeBeforeRun 
			{
				bool get() { 
					return safe_cast<OptionField<bool> ^>(field_list[FieldId::fiSaveCodeBeforeRun])->Value; 
				}
			}
			property bool PauseProgramAfterRun
			{
				bool get() { 
					return safe_cast<OptionField<bool> ^>(field_list[FieldId::fiPauseProgramAfterRun])->Value; 
				}
			}
			property bool PurgeOutputLog //tmp not used
			{
				bool get() { 
					return false; //!safe_cast<OptionField<bool> ^>(field_list[FieldId::fiNoPurgeOutputLog])->Value; //negation
				}
			}
			//visual tab properties
			property System::Drawing::Font ^ EditorFont
			{
				System::Drawing::Font ^ get() { 
					return safe_cast<OptionFieldEditor ^>(field_list[FieldId::fiEditor])->FontValue; 
				}
			}
			property System::Drawing::Color EditorForeColor
			{
				System::Drawing::Color get() { 
					return safe_cast<OptionFieldEditor ^>(field_list[FieldId::fiEditor])->EditorForeColorValue; 
				}
			}
			property System::Drawing::Color EditorBackColor
			{
				System::Drawing::Color get() { 
					return safe_cast<OptionFieldEditor ^>(field_list[FieldId::fiEditor])->EditorBackColorValue; 
				}
			}
			property System::Drawing::Color MarginForeColor
			{
				System::Drawing::Color get() { 
					return safe_cast<OptionFieldEditor ^>(field_list[FieldId::fiEditor])->MarginForeColorValue; 
				}
			}
			property System::Drawing::Color MarginBackColor
			{
				System::Drawing::Color get() { 
					return safe_cast<OptionFieldEditor ^>(field_list[FieldId::fiEditor])->MarginBackColorValue; 
				}
			}
			property int SyntaxStyle
			{
				int get() { 
					return safe_cast<OptionField<int> ^>(field_list[FieldId::fiSyntaxStyle])->Value; 
				}
			}
			//debugger tab
			property bool TraceNewThread
			{
				bool get() { 
					return safe_cast<OptionField<bool> ^>(field_list[FieldId::fiTraceNewThread])->Value; 
				}
			}
			property bool MemViewOnThreadCaptionClick
			{
				bool get() { 
					return safe_cast<OptionField<bool> ^>(field_list[FieldId::fiMemViewOnThreadCaptionClick])->Value; 
				}
			}
			property int DebugeeWindowStyle
			{
				int get() { 
					return safe_cast<OptionField<int> ^>(field_list[FieldId::fiDebugeeWindowStyle])->Value; 
				}
			}
			property int DebugeeWindowPosition
			{
				int get() { 
					return safe_cast<OptionField<int> ^>(field_list[FieldId::fiDebugeeWindowPosition])->Value; 
				}
			}

			//special
			property array<String^> ^ RecentFilesList
			{
				array<String^> ^ get() { 
					return recentFilesList->ToArray();
				}
			}


		private:
			bool isGenuineInterpreterSet(BrainthreadIDE::Language lang) {
				return System::IO::File::Exists( Instance->InterpreterPath[lang] + ".pdb");
			}
		public:
		    bool IsGenuineInterpreterPath(String ^ int_path) {
				return System::IO::File::Exists( int_path + ".pdb");
			}
	};
}