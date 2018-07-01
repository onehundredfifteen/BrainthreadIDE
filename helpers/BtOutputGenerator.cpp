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
            String ^ a = G[lastc, c];
            String ^ b = G[0, c];
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
		// initial state for G[x][y]: go from x to y using +s or -s.                                
        for (int x = 0; x < 256; x++) 
		{
            for (int y = 0; y < 256; y++) 
			{
                int delta = y - x;
                if (delta > 128) delta -= 256;
                if (delta < -128) delta += 256;

                if (delta >= 0) {
                    G[x,y] = gcnew String('+', delta);
                } else {
                    G[x,y] = gcnew String('-', -delta);
                }
            }
        }

        // keep applying rules until we can't find any more shortenings                             
        bool iter = true;
        while (iter) {
            iter = false;

            // multiplication by n/d                                                                
            for (int x = 0; x < 256; x++) {
                for (int n = 1; n < 40; n++) {
                    for (int d = 1; d < 40; d++) {
                        int j = x;
                        int y = 0;
                        for (int i = 0; i < 256; i++) {
                            if (j == 0) break;
                            j = (j - d + 256) & 255;
                            y = (y + n) & 255;
                        }
                        if (j == 0) {
                            String ^ s = "[" + gcnew String('-', d) + ">" + gcnew String('+', n) + "<]>";
                            if (s->Length < G[x, y]->Length) {
                                G[x, y] = s;
                                iter = true;
                            }
                        }

                        j = x;
                        y = 0;
                        for (int i = 0; i < 256; i++) {
                            if (j == 0) break;
                            j = (j + d) & 255;
                            y = (y - n + 256) & 255;
                        }
                        if (j == 0) {
                            String ^ s = "[" + gcnew String('+', d) + ">" + gcnew String('-', n) + "<]>";
                            if (s->Length < G[x,y]->Length) {
                                G[x,y] = s;
                                iter = true;
                            }
                        }
                    }
                }
            }

            // combine number schemes                                                               
            for (int x = 0; x < 256; x++) {
                for (int y = 0; y < 256; y++) {
                    for (int z = 0; z < 256; z++) {
                        if (G[x,z]->Length + G[z,y]->Length < G[x,y]->Length) {
                            G[x,y] = G[x,z] + G[z,y];
                            iter = true;
                        }
                    }
                }
            }
		}
	}

	
}