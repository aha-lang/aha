#include "AhaScaner.h"

#include <string.h>
#include <stdlib.h>
#include <cctype>
#include <locale>
#include <codecvt>

#include <fstream>

#include "Utility.h"

CAhaScaner::CAhaScaner(const std::string& _src, std::wstring& sourcef) : source(sourcef), src(_src)
{
	keywords.push_back(L"int");
	keywords.push_back(L"short");
	keywords.push_back(L"long");
	keywords.push_back(L"float");
	keywords.push_back(L"string");
	keywords.push_back(L"bool");
	keywords.push_back(L"void");
	keywords.push_back(L"for");
	keywords.push_back(L"while");
	keywords.push_back(L"do");
	keywords.push_back(L"switch");
	keywords.push_back(L"case");
	keywords.push_back(L"default");
	keywords.push_back(L"if");
	keywords.push_back(L"else");
	keywords.push_back(L"break");
	keywords.push_back(L"continue");
	keywords.push_back(L"sizeof");
	keywords.push_back(L"const");
	keywords.push_back(L"public");
	keywords.push_back(L"private");
	keywords.push_back(L"protected");
	keywords.push_back(L"internal");
	keywords.push_back(L"abstract");
	keywords.push_back(L"sealed");
	keywords.push_back(L"static");
	keywords.push_back(L"interface");
	keywords.push_back(L"import");
	keywords.push_back(L"true");
	keywords.push_back(L"false");
	keywords.push_back(L"class");
	keywords.push_back(L"virtual");
	keywords.push_back(L"operator");

	tnum.push_back(sym_integer);
	tnum.push_back(sym_short);
	tnum.push_back(sym_long);
	tnum.push_back(sym_float);
	tnum.push_back(sym_string);
	tnum.push_back(sym_boolean);
	tnum.push_back(sym_void);
	tnum.push_back(sym_for);
	tnum.push_back(sym_while);
	tnum.push_back(sym_do);
	tnum.push_back(sym_switch);
	tnum.push_back(sym_case);
	tnum.push_back(sym_default);
	tnum.push_back(sym_if);
	tnum.push_back(sym_else);
	tnum.push_back(sym_break);
	tnum.push_back(sym_continue);
	tnum.push_back(sym_sizeof);
	tnum.push_back(sym_const);
	tnum.push_back(sym_public);
	tnum.push_back(sym_private);
	tnum.push_back(sym_protected);
	tnum.push_back(sym_internal);
	tnum.push_back(sym_abstract);
	tnum.push_back(sym_sealed);
	tnum.push_back(sym_static);
	tnum.push_back(sym_interface);
	tnum.push_back(sym_import);
	tnum.push_back(sym_true);
	tnum.push_back(sym_false);
	tnum.push_back(sym_class);
	tnum.push_back(sym_virtual);
	tnum.push_back(sym_operator);
}

CAhaScaner::~CAhaScaner(void)
{
}

void CAhaScaner::Scan()
{
	TokenType token;
	unsigned int count = -1;
	int i, k;
	std::wstring id;
	wchar_t ch;
	unsigned long line = 1;

	if(source.empty())
		return ;
	
	do {
		count++;
		ch = source[count];

		if(superLetter(ch)) // 키워드와 변수 인식
		{
			i = 0;

			do {
				if(i < ID_LENGTH) id += ch;
				count++;
				ch = source[count];
			} while(superLetterOrDigit(ch));

			count--;
			ch = source[count];

			// 키워드인지 검사
			bool is_keyword = false;

			for(unsigned int n = 0; n < keywords.size(); n++)
				if(keywords[n] == id)
				{
					is_keyword = true;
					k = n;

					break;
				}

			if(is_keyword) // 키워드
			{
				token.number = tnum[k];
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
		else if(((ch == L'+' || ch == L'-') && iswdigit(source[count + 1])) || iswdigit(ch))
		{
			i = 0;

			do {
				if(i < ID_LENGTH) id += ch;
				count++;
				ch = source[count];
			} while(iswdigit(ch));

			// 정수일 경우
			if(ch != L'.')
			{
				count--;
				ch = source[count];

				token.number = tint;
				token.value = id;
				token.line = line;
				tokens.push_back(token);
			}
			else // 실수
			{
				do {
					if(i < ID_LENGTH) id += ch;
					count++;
					ch = source[count];
				} while(iswdigit(ch));

				count--;
				ch = source[count];

				token.number = tfloat;
				token.value = id;
				token.line = line;
				tokens.push_back(token);
			}
		}

		// 문자열 인식
		else if(ch == L'\"')
		{
			count++;
			ch = source[count];
			i = 0;

			do {
				if(ch == L'\\')
				{
					count++;
					ch = source[count];

					if(ch == L'\"')
					{
						id += L'\"';
						count++;
						ch = source[count];
						continue;
					}
				}

				id += ch;
				count++;
				ch = source[count];
			} while(ch != L'\"');

			token.number = tstring;
			token.value = id;
			token.line = line;
			tokens.push_back(token);
		}

		// 연산자와 문법기호 인식
		else
		{
			switch(ch)
			{
			case L'+':
				id += L'+';

				token.number = tplus;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'-':
				id += L'-';

				token.number = tminus;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'*':
				id += L'*';

				token.number = tmul;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'/':
				id += L'/';

				token.number = tdiv;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'=':
				id += L'=';

				token.number = tequal;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'{':
				id += L'{';

				token.number = tmlparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'}':
				id += L'}';

				token.number = tmrparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'.':
				id += L'.';

				token.number = tddot;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L',':
				id += L',';

				token.number = tcomma;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L';':
				id += L';';

				token.number = tsemicolon;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L':':
				id += L':';

				token.number = tddot2;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'(':
				id += L'(';

				token.number = tlparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L')':
				id += L')';

				token.number = trparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'[':
				id += L'[';

				token.number = tblparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L']':
				id += L']';

				token.number = tbrparan;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L'\n':
				line++;

				break;

			case L'!':
				id += '!';

				token.number = tnot;
				token.value = id;
				token.line = line;
				tokens.push_back(token);

				break;

			case L' ':
			case L'\t':
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
	} while(count < (int)source.size()-1);
}

void CAhaScaner::SaveToFile(const char* filename)
{
	std::wofstream file;

	file.open(filename, std::ios::out);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

	for (TokenType token : tokens)
	{
		file << L"Token Num = " << token.number;
		file << L", Value = " << token.value << std::endl;
	}
	
	file.close();
}

std::vector<TokenType>& CAhaScaner::get()
{
	return tokens;
}