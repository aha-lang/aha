#include "AhaParser.h"

#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>

#include "AhaError.h"

CAhaParser::CAhaParser(const string& _src, std::vector<TokenType> _tokens) : tokens(_tokens), src(_src)
{
}

CAhaParser::~CAhaParser()
{
}

void CAhaParser::Parse()
{
	eAccessType at = INTERNAL;
	eClassType ct = NORMAL;
	wstring tn = L"";
	wstring name = L"";
	
	for (unsigned int index = 0; index < tokens.size(); index++)
	{
		if (tokens[index].number == sym_import)
		{
			wstring refer;

			do
			{
				index++;

				if (refer == L"" && tokens[index].number == tsemicolon)
				{
					PrintError("AHA 0004", src, tokens[index].line);
					goto err_exit;
				}

				if (index == tokens.size()) // eof
				{
					PrintError("AHA 0001", src, tokens[index-1].line, L";");
					goto err_exit;
				}

				if (tokens[index].line != tokens[index - 1].line)
				{
					PrintError("AHA 0003", src, tokens[index].line, tokens[index].value);

					index--;
					goto err_exit;
				}
				
				refer += tokens[index].value;
			} while (tokens[index].number != tsemicolon);
			
			refers.push_back(refer);
		}
		else if (is_access(tokens[index].number))
		{
			switch (tokens[index].number)
			{
			case sym_public:
				at = PUBLIC;
				break;

			case sym_private:
				at = PRIVATE;
				break;

			case sym_protected:
				if (tokens[index + 1].number == sym_internal)
				{
					at = PROTECTED_INTERNAL;
					index++;
				}
				else
				{
					at = PROTECTED;
				}

				break;

			case sym_internal:
				at = INTERNAL;
				break;
			}
		}
		else if (is_class_type(tokens[index].number))
		{
			switch (tokens[index].number)
			{
			case sym_abstract:
				ct = ABSTRACT;
				break;

			case sym_sealed:
				ct = SEALED;
				break;

			case sym_static:
				ct = STATIC;
				break;

			case sym_interface:
				ct = INTERFACE;
				break;
			}
		}
		else if (tokens[index].number == sym_class)
		{
			index++;
			if (tokens[index].number == tident)
			{
				sClass cls;
				
				cls.at = at;
				cls.ct = ct;
				cls.name = tokens[index].value;

				at = INTERNAL;
				ct = NORMAL;

				classes.push_back(cls);
			}
		}
		else if (tokens[index].number == tident 
					|| is_type(tokens[index].number))
		{
			tn = tokens[index].value;

			index++;
			if (tokens[index].number == tident)
			{
				name = tokens[index].value;

				index++;
				if (tokens[index].number == tlparan) // func
				{
					bool istype = true;
					sFunction func;

					func.at = at;
					func.tn = tn;
					func.name = name;

					index++;
					while (tokens[index].number != trparan)
					{
						tn = tokens[index].value;

						index++;

						sVar param;

						param.at = INTERNAL;
						param.tn = tn;
						param.name = tokens[index].value;

						tn = L"";

						index++;

						if (tokens[index].number == tequal)
						{
							index++;

							param.init = tokens[index].value;
						}
						else if (tokens[index].number == trparan)
						{
							func.params.push_back(param);
							break;
						}

						func.params.push_back(param);
						
						index++;
					};

					index++;
					if (tokens[index].number == tmlparan)
					{
						index++;

						while (tokens[index].number != tmrparan)
						{
							if (tokens[index].number == tstring)
							{
								if (std::find(datas.begin(), datas.end(),
									tokens[index].value) == datas.end())
								{
									datas.push_back(tokens[index].value);
								}
							}

							func.inner.push_back(tokens[index]);

							index++;
						}

						sClass& cls = classes.back();
						cls.funcs.push_back(func);
					}
				}
				else if (tokens[index].number == tmlparan) // property
				{
					index++;

					do 
					{
						sFunction func;

						if (tokens[index].value == L"get") // get
						{
							index++;

							func.at = at;
							func.tn = tn;
							func.name = name + L"@@GET_get";

							sClass& cls = classes.back();

							if (tokens[index].number == tmlparan)
							{
								index++;

								while (tokens[index].number != tmrparan)
								{
									if (tokens[index].number == tstring)
									{
										if (std::find(datas.begin(), datas.end(),
											tokens[index].value) == datas.end())
										{
											datas.push_back(tokens[index].value);
										}
									}

									func.inner.push_back(tokens[index]);

									index++;
								}
							}
							else if (tokens[index].number == tsemicolon)
							{
								sVar var;
								var.at = at;
								var.name = L"__@@" + name;
								var.tn = tn;

								if (std::find(cls.vars.begin(), cls.vars.end(),
									var) == cls.vars.end())
								{
									cls.vars.push_back(var);
								}

								func.inner.push_back(
									TokenType(sym_return, tokens[index].line, L"return"));

								func.inner.push_back(
									TokenType(tident, tokens[index].line, L"__@@" + name));

								func.inner.push_back(
									TokenType(tsemicolon, tokens[index].line, L";"));
							}

							index++;
							cls.funcs.push_back(func);
						}
						else if (tokens[index].value == L"set") // set
						{
							index++;

							func.tn = tn;
							func.name = name + L"@@GET_set";
							func.params.push_back(sVar(PRIVATE, tn, L"value", L""));

							sClass& cls = classes.back();

							if (tokens[index].number == tmlparan)
							{
								index++;

								while (tokens[index].number != tmrparan)
								{
									if (tokens[index].number == tstring)
									{
										if (std::find(datas.begin(), datas.end(),
											tokens[index].value) == datas.end())
										{
											datas.push_back(tokens[index].value);
										}
									}

									func.inner.push_back(tokens[index]);

									index++;
								}
							}
							else if (tokens[index].number == tsemicolon)
							{
								sVar var;
								var.at = at;
								var.name = L"__@@" + name;
								var.tn = tn;

								if (std::find(cls.vars.begin(), cls.vars.end(),
									var) == cls.vars.end())
								{
									cls.vars.push_back(var);
								}

								func.inner.push_back(
									TokenType(tident, tokens[index].line, L"__@@" + name));

								func.inner.push_back(
									TokenType(tequal, tokens[index].line, L"="));

								func.inner.push_back(
									TokenType(tident, tokens[index].line, L"value"));

								func.inner.push_back(
									TokenType(tsemicolon, tokens[index].line, L";"));
							}

							index++;
							cls.funcs.push_back(func);
						}
					} while (tokens[index].number != tmrparan);
				}
				else // var
				{
					sVar var;

					while (1)
					{
						var.at = at;
						var.name = name;
						var.tn = tn;

						if (tokens[index].number == tequal)
						{
							index++;
							if (tokens[index].number == tstring)
							{
								var.init += L"\"";
								var.init += tokens[index].value;
								var.init += L"\"";
							}
							else
							{
								var.init = tokens[index].value;
							}
							index++;
						}

						sClass& cls = classes.back();
						cls.vars.push_back(var);

						at = INTERNAL;
						name = L"";
						tn = L"";
						
						if (tokens[index].number == tcomma)
							continue;
						else if (tokens[index].number == tsemicolon)
							break;
					}
				}
			}

		err_exit:
			continue;
		}
	}

	return;
}

void CAhaParser::SaveToFile(const char* filename)
{
	wofstream file;

	file.open(filename, ios::out);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

	file << L"refer" << endl;

	for (const wstring& refer : refers)
	{
		file << L"\"" << refer << L"\"" << endl;
	}

	file << endl << L"data" << endl;

	for (const wstring& data : datas)
	{
		file << L"\"" << data << L"\"" << endl;
	}

	file << endl << L"typeinfo" << endl;

	for (sClass cls : classes)
	{
		file << "class \"" << cls.name << "\", Access: ";

		file << AccessTypeStr[cls.at];

		file << ", Type: ";

		file << ClassTypeStr[cls.ct];

		file << endl;

		for (sVar var : cls.vars)
		{
			file << "var \"" << var.name << "\", Access: ";
			file << AccessTypeStr[var.at];
			file << ", Type: ";
			file << var.tn;

			if (var.init != L"")
				file << ", Init: " << var.init;

			file << endl;
		}

		for (sFunction func : cls.funcs)
		{
			file << "func \"" << func.name << "\", Access: ";
			file << AccessTypeStr[func.at];
			file << ", Ret-Type: ";
			file << func.tn;
			
			file << endl << "params" << endl;
			for (sVar param : func.params)
			{
				file << "param \"" << param.name << "\", Type: ";
				file << param.tn;

				if (param.init != L"")
					file << ", Init: " << param.init;

				file << endl;
			}

			file << "inner" << endl;
			for (TokenType token : func.inner)
			{
				file << token.value << " ";
			}

			file << endl;
		}
	}

	file.close();
}