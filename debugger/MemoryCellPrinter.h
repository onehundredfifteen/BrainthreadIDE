#pragma once

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Text;

	public ref class MemoryCellPrinter 
	{
	public: virtual String ^ Print(String ^ cell) 
			{
				return this->Print(Convert::ToInt32(cell));
			}
	public: virtual String ^ Print(int cell) 
			{
				return cell.ToString();
			}
	};

	public ref class UnsignedCellPrinter : public MemoryCellPrinter
	{
	public: virtual String ^ Print(int cell) override
			{
				return static_cast<unsigned int>(cell).ToString();
			}
	};

	public ref class CharacterCellPrinter : public MemoryCellPrinter
	{
	public: virtual String ^ Print(int cell) override
			{
				Encoding ^ encoding;
				short cell_val =  Convert::ToInt16(cell);

				if(cell_val == 0)//avoid printing null
				{
					return String::Empty;
				}
				else if(cell_val > 0xFF)//create wide char
				{
					encoding = Encoding::Unicode;
					return encoding->GetString( gcnew array<unsigned char>{ (unsigned char)cell_val, (unsigned char)(cell_val >> 8) }  );
				}
				else
				{
					encoding = Encoding::ASCII;
					return encoding->GetString( gcnew array<unsigned char>{ (unsigned char)cell_val } );
				}
			}
	};

	public ref class HexCellPrinter : public MemoryCellPrinter
	{
	public: virtual String ^ Print(int cell) override
			{
				return cell.ToString("X");
			}
	};

	public ref class MixedCellPrinter : public CharacterCellPrinter
	{
	public: virtual String ^ Print(int cell) override
			{
				wchar_t ch = cell;

				if(Char::IsControl(ch) || ch > 0xFF)
					return MemoryCellPrinter::Print(cell);
				else
					return CharacterCellPrinter::Print(cell);
			}
	
	};

	public ref class IntegerDigest : public MemoryCellPrinter
	{
	public:
			IntegerDigest(MemoryCellPrinter ^ mp) : memoryCellPrinter(mp), MemoryCellPrinter(){}

	public: virtual int Digest(String ^ cell) 
			{
				Int32 number;

				if(String::IsNullOrEmpty(cell))
					return 0;
				else if (Int32::TryParse(cell, number)) {
					return number;
				}
				else
					return (int)cell[0]; //return char
			}

	protected:
			MemoryCellPrinter ^ memoryCellPrinter;
	};
}