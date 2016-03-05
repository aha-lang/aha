#include "AhaScanner.h"

#include <fstream>
#include <cctype>
#include <locale>
#include <codecvt>

namespace
{
	void replaceStr(std::string & strCallId, const char * pszBefore, const char * pszAfter)
	{
		size_t iPos = strCallId.find(pszBefore);
		size_t iBeforeLen = strlen(pszBefore);
		while (iPos < std::string::npos)
		{
			strCallId.replace(iPos, iBeforeLen, pszAfter);
			iPos = strCallId.find(pszBefore, iPos);
		}
	}


	bool superLetter(char ch)
	{
		if (ch > 0xff) return true;
		else if (isalpha(ch) || ch == '_') return true;
		else return false;
	}

	bool superLetterOrDigit(char ch)
	{
		if (ch > 0xff) return true;
		else if (isalnum(ch) || ch == '_') return true;
		else return false;
	}
}

scanner::scanner(const source & src) : src(src)
{
}

void scanner::Scan()
{
	Token token;
	unsigned int count = -1;
	int i, k;
	std::string id;
	char ch;
	int line = 1;

	if (src.code.empty())
		return;

	do {
		count++;
		ch = src.code[count];

		if (superLetter(ch)) // 키워드와 변수 인식
		{
			i = 0;

			do {
				if (i < ID_LENGTH) id += ch;
				count++;
				ch = src.code[count];
			} while (superLetterOrDigit(ch));

			count--;
			ch = src.code[count];

			// 키워드인지 검사
			bool is_keyword = false;

			for (size_t n = 0; n < NUM_KEYWORDS; n++)
			{
				if (keywords[n] == id)
				{
					is_keyword = true;
					k = n;

					break;
				}
			}

			if (is_keyword) // 키워드
			{
				token.number = k + sym_void;
				token.value = id;
				token.line = line;
				tokens.push_back(token);
			}
			else
			{
				token.number = tident;
				token.value = id;
				token.line = line;
				tokens.push_back(token);
			}
		}

		// 정수와 실수 인식
		else if (((ch == '+' || ch == '-') && isdigit(src.code[count + 1])) || isdigit(ch))
		{
			i = 0;

			do {
				if (i < ID_LENGTH) id += ch;
				count++;
				ch = src.code[count];
			} while (iswdigit(ch));

			// 정수일 경우
			if (ch != L'.')
			{
				count--;
				ch = src.code[count];

				token.number = tint;
				token.value = id;
				token.line = line;
				tokens.push_back(token);
			}
			else // 실수
			{
				do {
					if (i < ID_LENGTH) id += ch;
					count++;
					ch = src.code[count];
				} while (iswdigit(ch));

				count--;
				ch = src.code[count];

				token.number = tfloat;
				token.value = id;
				token.line = line;
				tokens.push_back(token);
			}
		}

		// 문자열 인식
		else if (ch == '\"')
		{
			count++;
			ch = src.code[count];
			i = 0;

			do {
				if (ch == L'\\')
				{
					count++;
					ch = src.code[count];

					if (ch == L'\"')
					{
						id += L'\"';
						count++;
						ch = src.code[count];
						continue;
					}
				}

				id += ch;
				count++;
				ch = src.code[count];
			} while (ch != '\"');

			token.number = tstring;
			token.value = id;
			token.line = line;
			tokens.push_back(token);
		}

		// 연산자와 문법기호 인식
		else
		{
			switch (ch)
			{
			case '+':
				id += '+';

				token.number = tplus;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '-':
				id += '-';

				token.number = tminus;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '*':
				id += '*';

				token.number = tmul;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '/':
				id += '/';

				token.number = tdiv;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '=':
				id += '=';

				token.number = tequal;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '{':
				id += '{';

				token.number = tmlparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '}':
				id += '}';

				token.number = tmrparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '.':
				id += '.';

				token.number = tddot;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case ',':
				id += ',';

				token.number = tcomma;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case ';':
				id += ';';

				token.number = tsemicolon;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case ':':
				id += ':';

				token.number = tddot2;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '(':
				id += '(';

				token.number = tlparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case ')':
				id += ')';

				token.number = trparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '[':
				id += '[';

				token.number = tblparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case ']':
				id += ']';

				token.number = tbrparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case '\n':
				line++;

				break;

			case '!':
				id += '!';

				token.number = tnot;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case ' ':
			case '\t':
				break;

			default:
				id += ch;

				token.number = tnull;
				token.value = id;
				token.line = line;
				tokens.push_back(token);
			}
		}

		id.clear();
	} while (count < src.code.size() - 1);
}

void scanner::SaveToFile(const std::string & filename)
{
	std::ofstream file;

	file.open(filename, std::ios::out);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

	for (Token token : tokens)
	{
		file << L"Token Num = " << token.number;
		file << L", Value = " << token.value << std::endl;
	}

	file.close();
}

std::vector<Token>& scanner::Get()
{
	return tokens;
}
