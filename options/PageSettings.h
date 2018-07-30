#pragma once

#include "Language.h"

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Data;
	using namespace System::Windows::Forms;
	using namespace System::Xml;

	public ref class PageSettings
	{
	public:
		PageSettings(void)
		{
			this->Default();
		}

		PageSettings(const PageSettings ^ p) 
		{
			this->selLanguage = p->selLanguage;
			this->selCellsize = p->selCellsize;
			this->selMemoryBehavior = p->selMemoryBehavior;
			this->selEofBehavior = p->selEofBehavior;
			this->selMemorySize = p->selMemorySize;
			this->selAdditionalCommands = p->selAdditionalCommands;
			this->selInput = p->selInput;
			this->selRedirectStreams = p->selRedirectStreams;
		}

	private:
		//selections
		BrainthreadIDE::Language selLanguage;
		int selCellsize;
		int selMemoryBehavior;
		int selEofBehavior;
		unsigned int selMemorySize;
		String ^ selAdditionalCommands;
		String ^ selInput;
		bool selRedirectStreams;

	public:
		static void Init(ComboBox ^ cLanguage, ComboBox ^ cCellsize, ComboBox ^ cMemoryBehavior,
			                     ComboBox ^ cEofBehavior, TextBox ^ cMemorySize, TextBox ^ cAdditionalCommands,
								 TextBox ^ cInputText, CheckBox ^ cRedirectStreams);

		void Save(void);
		void Load(void);
		void Default(void);

		String ^ GetRunString(void);
		String ^ GetCustomRunString(void);
		String ^ GetAdditionalCommandsString(void);
		String ^ GetInputString(void);
		
		BrainthreadIDE::Language GetLanguage(void);
		bool GetRedirectionOption(void);
		
		void WriteOptions(XmlDocument^ xmldoc);
		void ReadOptions(XmlDocument^ xmldoc);

    private:
		void PopulateXMLNodesForOptions(XmlDocument^ xmldoc);

	private:
		static DataTable ^ dtCellsize;
		static DataTable ^ dtMemoryBehavior;
		static DataTable ^ dtEofBehavior;
		
		//controls
		static ComboBox ^ ctLanguage;
		static ComboBox ^ ctCellsize;
		static ComboBox ^ ctMemoryBehavior;
		static ComboBox ^ ctEofBehavior;
		static TextBox ^ ctMemorySize;
		static TextBox ^ ctAdditionalCommands;
		static TextBox ^ ctInputText;
		static CheckBox ^ ctRedirectStreams;

		//constans headers
		literal String ^ dtCellsizeDisplayHeader = "cellsizeDecs";
		literal String ^ dtCellsizeValueHeader = "cellsizeOps";
		literal String ^ dtMemoryBehaviorDisplayHeader = "memDecs";
		literal String ^ dtMemoryBehaviorValueHeader = "memOps";
		literal String ^ dtEofBehaviorDisplayHeader = "eofDecs";
		literal String ^ dtEofBehaviorValueHeader = "eofOps";

		//xml nodes
		literal String ^ cxmlnodeLanguage = "language";
		literal String ^ cxmlnodeCellsize = "cellsize";
		literal String ^ cxmlnodeMemoryBehavior = "memory";
		literal String ^ cxmlnodeMemorySize = "memorySize";
		literal String ^ cxmlnodeEofBehavior = "eof";
		literal String ^ cxmlnodesAdditionalCommands = "commands";
		literal String ^ cxmlnodeInput = "input";
		literal String ^ cxmlnodeProject = "btide_project";

		//xml attributes
		literal String ^ cxmlnodeattrStreamRedirection = "stream_redirection";

	};
}

