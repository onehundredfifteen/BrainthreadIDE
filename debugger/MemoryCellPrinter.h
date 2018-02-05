#pragma once

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Text;

	public ref class MemoryCellPrinter 
	{
	public: virtual String ^ Print(String ^ cell) 
			{
				return cell;
			}
	public: virtual String ^ Print(int cell) 
			{
				return cell.ToString();
			}
	
	};

	public ref class CharacterCellPrinter : public MemoryCellPrinter
	{
	public: virtual String ^ Print(String ^ cell) override
			{
				Encoding^ ascii = Encoding::ASCII;
				return ascii->GetString( gcnew array<unsigned char>{ Convert::ToInt32(cell) } );
			}
	public: virtual String ^ Print(int cell) override
			{
				Encoding^ ascii = Encoding::ASCII;
				return ascii->GetString( gcnew array<unsigned char>{ cell } );
			}
	};

	public ref class HexCellPrinter : public MemoryCellPrinter
	{
	public: virtual String ^ Print(String ^ cell) override
			{
				int c = Convert::ToInt32(cell);
				return c.ToString("X");
			}
	public: virtual String ^ Print(int cell) override
			{
				return cell.ToString("X");
			}
	};

	public ref class BulkPrinter : public MemoryCellPrinter
	{//not used
	public: virtual String ^ Print(String ^ cell) override
			{
				int c = Convert::ToInt32(cell);
				return String::Format("x{0}", c.ToString("X"));
			}
	
	};
}