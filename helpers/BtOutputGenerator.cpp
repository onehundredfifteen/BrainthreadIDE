#include "StdAfx.h"
#include "BtOutputGenerator.h"

namespace BrainthreadIDE 
{	
	String ^ BtOutputGenerator::Generate(String ^ str) 
	{
        char lastc = 0;
		StringBuilder ^ sb = gcnew StringBuilder();

        for each(char c in str) 
		{
            String ^ a = Template[lastc, c];
            String ^ b = Template[0, c];
            if (a->Length <= b->Length) {
                sb->Append(a);
            } else {
                sb->Append(">");
				sb->Append(b);
            }
            sb->Append(".");
            lastc = c;
        }

		return sb->ToString();
	}

	void BtOutputGenerator::Init()
	{
		// initial state for Template[x][y]: go from x to y using +s or -s.                                
        for (int x = 0; x < tp_len; x++) 
		{
            for (int y = 0; y < tp_len; y++) 
			{
                int delta = y - x;
                if (delta > 128) delta -= tp_len;
                if (delta < -128) delta += tp_len;

                if (delta >= 0) {
                    Template[x,y] = gcnew String('+', delta);
                } else {
                    Template[x,y] = gcnew String('-', -delta);
                }
            }
        }

        // keep applying rules until we can't find any more shortenings                             
        bool iter = true;
        while (iter) {
            iter = false;

            // multiplication by n/d                                                                
            for (int x = 0; x < tp_len; x++) {
                for (int n = 1; n < 40; n++) {
                    for (int d = 1; d < 40; d++) {
                        int j = x;
                        int y = 0;
                        for (int i = 0; i < tp_len; i++) {
                            if (j == 0) break;
                            j = (j - d + tp_len) & 255;
                            y = (y + n) & 255;
                        }
                        if (j == 0) {
                            String ^ s = "[" + gcnew String('-', d) + ">" + gcnew String('+', n) + "<]>";
                            if (s->Length < Template[x, y]->Length) {
                                Template[x, y] = s;
                                iter = true;
                            }
                        }

                        j = x;
                        y = 0;
                        for (int i = 0; i < tp_len; i++) {
                            if (j == 0) break;
                            j = (j + d) & 255;
                            y = (y - n + tp_len) & 255;
                        }
                        if (j == 0) {
                            String ^ s = "[" + gcnew String('+', d) + ">" + gcnew String('-', n) + "<]>";
                            if (s->Length < Template[x,y]->Length) {
                                Template[x,y] = s;
                                iter = true;
                            }
                        }
                    }
                }
            }

            // combine number schemes                                                               
            for (int x = 0; x < tp_len; x++) {
                for (int y = 0; y < tp_len; y++) {
                    for (int z = 0; z < tp_len; z++) {
                        if (Template[x,z]->Length + Template[z,y]->Length < Template[x,y]->Length) {
                            Template[x,y] = Template[x,z] + Template[z,y];
                            iter = true;
                        }
                    }
                }
            }
		}
	}

	void BtOutputGenerator::InitFromMemory(FileContext ^ memFile)
	{
		array<Char>^ sep_1 = gcnew array<Char>{'\n'};
		//array<Char>^ sep_2 = gcnew array<Char>{'\t'};
		array<String ^> ^ conctenatedArr = memFile->Content->Split( sep_1, tp_len * tp_len, StringSplitOptions::None );

		for (int x = 0; x < tp_len; x++) {
			for (int y = 0; y < tp_len; y++)
				Template[x,y] = conctenatedArr[x * tp_len + y];
		}
	}

	void BtOutputGenerator::SaveToMemory(FileContext ^ memFile)
	{
		StringBuilder ^ sb = gcnew StringBuilder();

		for (int x = 0; x < tp_len; x++) {
			for (int y = 0; y < tp_len; y++)
				sb->Append(Template[x,y] + "\n");
		}

		memFile->Content = sb->ToString();
		memFile->Save();
	}

	
}