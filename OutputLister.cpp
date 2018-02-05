#include "StdAfx.h"
#include "OutputLister.h"
#include "WorkContexts.h"
#include "helpers/CodeContextConverter.h"

namespace BrainthreadIDE 
{
	void OutputLister::Refresh()
	{
		//textbox
		outputBox->Lines = outputStrings->ToArray();

		//listview
		outputList->Items->Clear();
		outputItems->Clear();

		for each(String ^ line in outputStrings)
		{
			OutputLister::OutputItem ^item = this->ParseOutput(line);

			if(item)
			{
				ListViewItem ^lvi = gcnew ListViewItem(item->kind);
				lvi->SubItems->Add(item->instruction.ToString());
				lvi->SubItems->Add(item->description);

				if(item->important)
					outputList->Items->Insert(0, lvi);
				else
					outputList->Items->Add(lvi);

				outputItems->Add(lvi, item);
			}
		}

		outputList->AutoResizeColumns(ColumnHeaderAutoResizeStyle::ColumnContent); 
		outputList->AutoResizeColumns(ColumnHeaderAutoResizeStyle::HeaderSize);
	}

	void OutputLister::Purge()
	{
		//if(GlobalOptions::Instance->PurgeOutputLog) {
			outputStrings->Clear();
			outputBox->Clear();		
		//}
		//else
			//this->AddOutput(gcnew System::Collections::Generic::List<String ^>(gcnew array<String ^>(3){"", gcnew String('-', 15), ""} ));

		outputList->Items->Clear();
		outputItems->Clear();
	}

	void OutputLister::AddOutput(System::Collections::Generic::List<String ^> ^ oStrings)
	{
		outputStrings->AddRange(oStrings);
		this->Refresh();
	}

	void OutputLister::AddOutput(String^ line)
	{
		outputStrings->Add(line);
		this->Refresh();
	}

	void OutputLister::AddIDEOutput(String^ line)
	{
		this->AddOutput(String::Format("> {0}", line));
	}

	void OutputLister::AddOutputWithTimestamp(String ^ line)
	{
		this->AddOutput(String::Format("<info at {0}> {1}\r\n", DateTime::Now.ToLongTimeString(), line));
	}

	//funkcja podaje pozycje instrukcji w tekscie uzywajac jako slownika kluczy z syntax higligthera
	int OutputLister::GetErrorPosition(ListViewItem ^ listItem)
	{
		if(outputItems->ContainsKey(listItem))
		{
			CodeContextConverter ^ ccc = gcnew CodeContextConverter( WorkContextBroker::Instance->GetCurrentContext() );
			OutputLister::OutputItem ^ item = outputItems[ listItem ];

			return  ccc->ToCursor(item->instruction);
		}

		return 0;
	}
	
	OutputLister::OutputItem ^ OutputLister::ParseOutput(String ^ line)
	{
		OutputLister::OutputItem ^item = gcnew OutputLister::OutputItem;

		if(line->StartsWith(cError))
		{
			item->kind = cError;
			item->instruction = GetInstructionFromLine(line);
			item->description = line->Substring(line->IndexOf(':') + 1, line->IndexOf(cInstructionAt) - line->IndexOf(':') - 1)->Trim();
		}
		else if(line->StartsWith(cWarning))
		{
			item->kind = cWarning;
			item->instruction = GetInstructionFromLine(line);
			item->description = line->Substring(line->IndexOf(':') + 1, line->IndexOf(cInstructionAt) - line->IndexOf(':') - 1)->Trim();
		}
		else if(line->StartsWith(cMessage))
		{
			item->kind = cMessage;
			item->instruction = 0;
			item->description = line->Substring(line->IndexOf(':') + 1)->Trim();

			if(line->Contains(cCodeHas))//warnings or errors goes on top of list
				item->important = true;
		}
		else if(line->Contains(cCodeValid) || line->Contains(cCodeSane))
		{
			item->kind = cMessage;
			item->instruction = 0;
			item->description = line->Substring(line->IndexOf(':') + 1)->Trim();
			item->important = true;
		}
		else if(line->Contains(cTooManyWarnings))
		{
			item->kind = cWarning;
			item->instruction = 0;
			item->description = cTooManyWarnings;
			item->important = true;
		}
		else
		{
			return nullptr;
		}

		return item;
	}

	int OutputLister::GetInstructionFromLine(String ^ line)
	{
		if(line->Contains("at instruction"))
		{
			return Convert::ToInt32(line->Substring(line->LastIndexOf(' ')));
		}

		return 0;
	}

}
