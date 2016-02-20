#include "stdafx.h"
#include "Parser.h"

std::wstring without_comment(std::wstring &str);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s [source] [output]\n", argv[1]);
		return -1;
	}

	std::wifstream source;
	std::wstring strLine, strRead;
	unsigned CountOfLine = 1;
	bool bFinished = false;

	Parser parser;

	// 에러시 예외를 던지도록 함.
	source.exceptions(std::ios::failbit | std::ios::badbit);
	try
	{
		source.open(argv[1]);

		while (1)
		{
			std::getline(source, strRead);
			strLine += without_comment(strRead);

			if (!source.eof())
			{
				if (!bFinished)
				{
					if (!strLine.empty())
					{
						// 행 계속 문자
						if (strLine.back() == L'\\')
						{
							strLine.pop_back();
							goto LoopContinue;
						}
						else
						{
							if (parser.Line(strLine))
								bFinished = true;
							strLine.clear();
						}
					}
				}
				else if (!strLine.empty())
				{
					std::cerr << "line " << CountOfLine << ": last line is not empty" << std::endl;
					return -4;
				}

LoopContinue:
				CountOfLine++;
			}
			else
			{
				if (bFinished)
				{
					if (!strLine.empty())
					{
						std::cerr << "line " << CountOfLine << ": last line is not empty" << std::endl;
						return -4;
					}
					else
					{
						try
						{
							parser.Save(argv[2]);
							return 0;
						}
						catch (ParseError &e)
						{
							std::wcerr << e.what();
							std::cout << " : " << argv[2] << std::endl;
							return -5;
						}
					}
				}
				else
				{
					std::cerr << "line " << CountOfLine << ": unexpected End-Of-File" << std::endl;
					return -2;
				}
			}
		}
	}
	catch (ParseError &e)
	{
		std::wcerr << L"line " << CountOfLine << L": " << e.what() << std::endl;
		return 1;
	}
	catch (std::ios_base::failure &e)
	{
		std::cerr << "file read error : " << e.what() << std::endl;
		return -3;
	}
}

std::wstring without_comment(const std::wstring &str)
{
	bool single_quote = false;
	bool double_quote = false;

	for (int idx = str.size() - 1; idx > 0; --idx)
	{
		if (str[idx] == L'\'')
		{
			single_quote = !single_quote;
		}
		else if (str[idx] == L'"')
		{
			double_quote = !double_quote;
		}
		else if (str[idx] == L'#')
		{
			return str.substr(0, idx);
		}
	}
	return str;
}
