#include "StdAfx.h"
#include "PageSettings.h"
#include "GlobalOptions.h"

namespace BrainthreadIDE 
{
	PageSettings::PageSettings(void)
	{
		this->Default();
	}

	void PageSettings::Init(ComboBox ^ cLanguage, ComboBox ^ cCellsize, ComboBox ^ cMemoryBehavior,
			                         ComboBox ^ cEofBehavior, TextBox ^ cMemorySize, TextBox ^ cAdditionalCommands,
									 TextBox ^ cInputText, CheckBox ^ cRedirectStreams)
	{
		dtCellsize = gcnew DataTable();
		dtMemoryBehavior = gcnew DataTable();
		dtEofBehavior = gcnew DataTable();

		dtCellsize->Locale = System::Globalization::CultureInfo::InvariantCulture;
		dtMemoryBehavior->Locale = System::Globalization::CultureInfo::InvariantCulture;
		dtEofBehavior->Locale = System::Globalization::CultureInfo::InvariantCulture;

		//cell size options
		DataColumn ^ dtcCellsizeDesc = gcnew DataColumn( dtCellsizeDisplayHeader );
		DataColumn ^ dtcCellsizeOps = gcnew DataColumn( dtCellsizeValueHeader );		
			
		dtCellsize->Columns->Add(dtcCellsizeDesc);
		dtCellsize->Columns->Add(dtcCellsizeOps);

		dtCellsize->Rows->Add("8-bit unsigned (0; 255)", "-c u8");
		dtCellsize->Rows->Add("8-bit signed (-128; 127)", "-c 8");
		dtCellsize->Rows->Add("16-bit unsigned (0; 65535)", "-c u16");
		dtCellsize->Rows->Add("16-bit signed (-32768; 32767)", "-c 16");
		dtCellsize->Rows->Add("32-bit unsigned (0; 2^32)", "-c u32");
		dtCellsize->Rows->Add("32-bit signed (-2^32; 2^32-1)", "-c 32");

		//memory behavior options
		DataColumn ^ dtcMemorybehaviorDesc = gcnew DataColumn( dtMemoryBehaviorDisplayHeader );
		DataColumn ^ dtcMemorybehaviorOps = gcnew DataColumn( dtMemoryBehaviorValueHeader );		
			
		dtMemoryBehavior->Columns->Add(dtcMemorybehaviorDesc);
		dtMemoryBehavior->Columns->Add(dtcMemorybehaviorOps);

		dtMemoryBehavior->Rows->Add("Constant array", "-b constant");
		dtMemoryBehavior->Rows->Add("Growing array", "-b dynamic");
		dtMemoryBehavior->Rows->Add("Loop", "-b tapeloop");

		//eof options
		DataColumn ^ dtEofbehaviorDesc = gcnew DataColumn( dtEofBehaviorDisplayHeader );
		DataColumn ^ dtEofbehaviorOps = gcnew DataColumn( dtEofBehaviorValueHeader );		
			
		dtEofBehavior->Columns->Add(dtEofbehaviorDesc);
		dtEofBehavior->Columns->Add(dtEofbehaviorOps);

		dtEofBehavior->Rows->Add("Default", "-e 255");
		dtEofBehavior->Rows->Add("0", "-e 0");
		dtEofBehavior->Rows->Add("-1", "-e 255");
		dtEofBehavior->Rows->Add("no change", "-e nochange");

		//control init
		ctLanguage = cLanguage;
		ctCellsize = cCellsize;
		ctMemoryBehavior = cMemoryBehavior;
			                         
		ctEofBehavior	= cEofBehavior;
		ctMemorySize = cMemorySize;
		ctAdditionalCommands = cAdditionalCommands;

		ctInputText = cInputText;
		ctRedirectStreams = cRedirectStreams;
		
		//data source init
		ctLanguage->Items->Clear();
		for(int l = 0; l < (int)Language::l__spare; l++)
		{
			ctLanguage->Items->Add( LanguageName::GetLanguageName( (BrainthreadIDE::Language) l));
		}

		ctCellsize->DataSource = dtCellsize;
		ctCellsize->DisplayMember = dtCellsizeDisplayHeader;
		ctCellsize->ValueMember = dtCellsizeValueHeader;

		ctMemoryBehavior->DataSource = dtMemoryBehavior;
		ctMemoryBehavior->DisplayMember = dtMemoryBehaviorDisplayHeader;
		ctMemoryBehavior->ValueMember = dtMemoryBehaviorValueHeader;

		ctEofBehavior->DataSource = dtEofBehavior;
		ctEofBehavior->DisplayMember = dtEofBehaviorDisplayHeader;
		ctEofBehavior->ValueMember = dtEofBehaviorValueHeader;
	}

	void PageSettings::Save(void)
	{
		selLanguage = (BrainthreadIDE::Language)ctLanguage->SelectedIndex;
		selCellsize = ctCellsize->SelectedIndex;
		selMemoryBehavior = ctMemoryBehavior->SelectedIndex;
		selEofBehavior = ctEofBehavior->SelectedIndex;

		try
		{
			selMemorySize = Convert::ToInt32(ctMemorySize->Text);
		}
		catch(FormatException ^)
		{
			selMemorySize = 30000;
			ctMemorySize->Text = "30000";
		}

		selAdditionalCommands = ctAdditionalCommands->Text->Trim();

		selRedirectStreams = ctRedirectStreams->Checked;
		
		if(selRedirectStreams == false)
		    selInput = "";
		else
			selInput = ctInputText->Text->Trim();
	}

	void PageSettings::Load(void)
	{
		ctLanguage->SelectedIndex = (int)selLanguage;
		ctCellsize->SelectedIndex = selCellsize;
		ctMemoryBehavior->SelectedIndex = selMemoryBehavior;
		ctEofBehavior->SelectedIndex = selEofBehavior;
		ctMemorySize->Text = Convert::ToString(selMemorySize);

		ctAdditionalCommands->Text = selAdditionalCommands;
		ctInputText->Text = selInput;
		ctRedirectStreams->Checked = selRedirectStreams;
	}

	void PageSettings::Default(void)
	{
		selLanguage = Language::lBrainthread;
		selCellsize = 0;
		selMemoryBehavior = 0;
		selEofBehavior = 0;
		selMemorySize = 30000;
		selAdditionalCommands = "";
		selRedirectStreams = false;
		selInput = "";
	}

	String ^ PageSettings::GetRunString(void)
	{
		System::Text::StringBuilder ^ runStringBuilder = gcnew System::Text::StringBuilder();

		runStringBuilder->Append(GlobalOptions::Instance->InterpreterArgs[ (BrainthreadIDE::Language)ctLanguage->SelectedIndex] );	
		runStringBuilder->Append(" ");
		runStringBuilder->Append(String::Concat("-m ", selMemorySize.ToString()));
		runStringBuilder->Append(" ");
		runStringBuilder->Append(ctCellsize->SelectedValue);
		runStringBuilder->Append(" ");
		runStringBuilder->Append(ctMemoryBehavior->SelectedValue);
		runStringBuilder->Append(" ");
		runStringBuilder->Append(ctEofBehavior->SelectedValue);
		runStringBuilder->Append(" ");
		runStringBuilder->Append(this->GetAdditionalCommandsString());
	
		return runStringBuilder->ToString();
	}

	String ^ PageSettings::GetCustomRunString(void)
	{
		System::Text::StringBuilder ^ runStringBuilder = gcnew System::Text::StringBuilder();

		runStringBuilder->Append(GlobalOptions::Instance->InterpreterArgs[ (BrainthreadIDE::Language)ctLanguage->SelectedIndex] );	
		runStringBuilder->Append(" ");
		runStringBuilder->Append(this->GetAdditionalCommandsString());
	
		return runStringBuilder->ToString();
	}

	String ^ PageSettings::GetAdditionalCommandsString(void)
	{
		return ctAdditionalCommands->Text->Trim();
	}

	String ^ PageSettings::GetInputString(void)
	{
		if(selRedirectStreams == false)
			return nullptr;

		return selInput;
	}

	BrainthreadIDE::Language PageSettings::GetLanguage(void)
	{
		return selLanguage;
	}

	bool PageSettings::GetRedirectionOption(void)
	{
		return selRedirectStreams;
	}

	void PageSettings::WriteOptions(XmlDocument^ xmldoc)
	{
		XmlElement ^ nodeRoot = xmldoc->DocumentElement;
		this->Save();

		if(nodeRoot->ChildNodes->Count <= 2)
		{
			this->PopulateXMLNodesForOptions(xmldoc); 
		}

		for each(XmlElement ^ node in nodeRoot->ChildNodes)
        {
            if(node->Name == cxmlnodeLanguage ){
				node->InnerText =  ((int)selLanguage).ToString();
			}
			else if(node->Name == cxmlnodeCellsize ){
				node->InnerText =  selCellsize.ToString();
			}
			else if(node->Name == cxmlnodeMemoryBehavior ){
				node->InnerText =  selMemoryBehavior.ToString();
			}
			else if(node->Name == cxmlnodeMemorySize ){
				node->InnerText =  selMemorySize.ToString();
			}
			else if(node->Name == cxmlnodeEofBehavior ){
				node->InnerText =  selEofBehavior.ToString();
			}
			else if(node->Name == cxmlnodesAdditionalCommands ){
				node->InnerText =  selAdditionalCommands;
			}
			else if(node->Name == cxmlnodeInput ){
				node->InnerText =  selInput;
				node->SetAttribute(cxmlnodeattrStreamRedirection, selRedirectStreams.ToString());
			}
        }
	}

	void PageSettings::ReadOptions(XmlDocument^ xmldoc)
	{
		XmlElement ^ nodeRoot = xmldoc->DocumentElement;

		for each(XmlElement ^ node in nodeRoot->ChildNodes)
        {
            if(node->Name == cxmlnodeLanguage ){

				int lang = Convert::ToInt16(node->InnerText); //not 32
				if(lang >= (int)Language::l__spare)
				{
					MessageBox::Show("This language is not supported. Setting Brainfuck as project's language.","Unkown language", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				    selLanguage = Language::lBrainfuck;
				}
				else 
					selLanguage = (BrainthreadIDE::Language)lang;
			}
			else if(node->Name == cxmlnodeCellsize ){
				selCellsize = Convert::ToInt16(node->InnerText);
			}
			else if(node->Name == cxmlnodeMemoryBehavior ){
				selMemoryBehavior = Convert::ToInt16(node->InnerText);
			}
			else if(node->Name == cxmlnodeMemorySize ){
				selMemorySize = Convert::ToInt16(node->InnerText);
			}
			else if(node->Name == cxmlnodeEofBehavior ){
				selEofBehavior = Convert::ToInt16(node->InnerText);
			}
			else if(node->Name == cxmlnodesAdditionalCommands ){
				selAdditionalCommands = node->InnerText;
			}
			else if(node->Name == cxmlnodeInput ){
				selInput = node->InnerText;
				if (node->HasAttribute(cxmlnodeattrStreamRedirection) ){
					  selRedirectStreams = Convert::ToBoolean(node->GetAttribute( cxmlnodeattrStreamRedirection ));
				}
			}
        }
	}

	void PageSettings::PopulateXMLNodesForOptions(XmlDocument^ xmldoc)
	{
		XmlElement ^ nodeInput;
		XmlElement ^ nodeRoot = xmldoc->DocumentElement;

		if(nodeRoot->Name == cxmlnodeProject)
		{
			nodeRoot->AppendChild( xmldoc->CreateElement(cxmlnodeLanguage) );
			nodeRoot->AppendChild( xmldoc->CreateElement(cxmlnodeCellsize) );
			nodeRoot->AppendChild( xmldoc->CreateElement(cxmlnodeMemoryBehavior) );
			nodeRoot->AppendChild( xmldoc->CreateElement(cxmlnodeMemorySize) );
			nodeRoot->AppendChild( xmldoc->CreateElement(cxmlnodeEofBehavior) );
			nodeRoot->AppendChild( xmldoc->CreateElement(cxmlnodesAdditionalCommands) );

			nodeInput = xmldoc->CreateElement(cxmlnodeInput);
			nodeInput->SetAttribute(cxmlnodeattrStreamRedirection, String::Empty);
			nodeRoot->AppendChild( nodeInput );
		}
	}
}
