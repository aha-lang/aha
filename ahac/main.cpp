#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>

#include <time.h>

#include "AhaScaner.h"
#include "AhaParser.h"
#include "AhaCompiler.h"
#include "AhaLangPack.h"

using namespace std;

AhaLangPack err_lang;

int main(int argc, char** argv)
{
	wifstream file;

	vector<string> sources;
	string asmfile;
	string objfile;

	bool save_temp = false;

	setlocale(LC_ALL, "");

	err_lang.Load("./lang/error.lang");
	
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
		{
			if (argc >= i + 1)
			{
				i++;
				
				objfile = argv[i];
			}
		}
		else if (strcmp(argv[i], "-S") == 0)
		{
			if (argc >= i + 1)
			{
				i++;

				asmfile = argv[i];
			}
		}
		else if (strcmp(argv[i], "--save-temps") == 0)
		{
			save_temp = true;
		}
		else
		{
			sources.push_back(argv[i]);
		}
	}

	for (const string& src : sources)
	{
		file.open(src, ios::in);
		file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

		if (file.is_open())
		{
			wstring source;

			while (!file.eof())
			{
				wstring temp;
				std::getline(file, temp);
				temp += L"\n";

				source.append(temp);
			}

			CAhaScaner scaner(src, source);
			scaner.Scan();

			CAhaParser parser(src, scaner.get());
			parser.Parse();

			if (save_temp)
			{
				scaner.SaveToFile((src + ".s").c_str());
				parser.SaveToFile((src + ".p").c_str());
			}

			CAhaCompiler compiler(parser);

			if(!asmfile.empty())
				compiler.MakeAHSM(asmfile.c_str());
		}
	}

	return 0;
}