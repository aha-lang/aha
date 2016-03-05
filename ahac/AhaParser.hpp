#pragma once

#include "AhaCompiler.h"
#include "AhaTokens.h"
#include "AhaType.h"

#include <fstream>
#include <cctype>
#include <locale>
#include <codecvt>

class parser
{
public:
	parser(compile_context& _ctx, const source& _src, std::vector<Token> _tokens)
		: src(_src), ctx(_ctx), tokens(_tokens) { }

	void Parse()
	{
		AccessType at = INTERNAL;
		ClassType ct = NORMAL;

		Token tn;
		std::string name;

		size_t mparan_cnt = 0;

		for (size_t i = 0; i < tokens.size(); i++)
		{
			if (tokens[i].number == sym_import)
			{
				std::string refer;

				do
				{
					i++;

					if (tokens[i].number != tddot && tokens[i].number != tident)
					{
						if (tokens[i].number == tsemicolon) break;
						throw compiler_error("알 수 없는 토큰입니다.", code_position(src.file, tokens[i].line));
					}

					if (refer.empty() && tokens[i].number == tsemicolon)
					{
						throw compiler_error("인수가 부족합니다.", code_position(src.file, tokens[i].line));
					}

					if (tokens[i].line != tokens[i - 1].line || i == tokens.size()) // eof
					{
						throw compiler_error("';'이 없습니다.", code_position(src.file, tokens[i - 1].line));
					}

					refer += tokens[i].value;
				} while (tokens[i].number != tsemicolon);

				ctx.add_refer(refer);
			}
			else if IS_ACESS_TYPE(tokens[i].number)
			{
				switch (tokens[i].number)
				{
				case sym_internal: at = INTERNAL; break;
				case sym_private: at = PRIVATE; break;
				case sym_public: at = PUBLIC; break;
				case sym_protected:
					if (tokens[i + 1].number == sym_internal)
					{
						at = PROCTED_INTERNAL;
						i++;
					}
					else
					{
						at = PROTECTED;
					}
					break;
				}
			}
			else if IS_CLASS_TYPE(tokens[i].number)
			{
				switch (tokens[i].number)
				{
				case sym_abstract: ct = ABSTRACT; break;
				case sym_sealed: ct = SEALED; break;
				case sym_static: ct = STATIC; break;
				case sym_interface: ct = INTERFACE; break;
				}
			}
			else if (tokens[i].number == sym_class)
			{
				i++;
				Class cls;

				if (tokens[i].number == tident)
				{
					cls.at = at;
					cls.ct = ct;

					do
					{
						cls.name += tokens[i].value;
						i++;
					} while (tokens[i + 1].number == tddot || tokens[i + 1].number == tident);

					at = INTERNAL;
					ct = NORMAL;

					if (tokens[i + 1].number == tddot2)
					{
						do
						{
							std::string parent;
							do
							{
								i++;
								do
								{
									i++;
									parent += tokens[i].value;
								} while (tokens[i + 1].number == tddot || tokens[i + 1].number == tident);
							} while (tokens[i].number != tcomma);

							cls.parents.push_back(parent);
							parent.clear();
						} while (tokens[i].number != tmlparan);

						if (cls.parents.empty())
						{
							throw compiler_error("부모 클래스가 지정되지 않았습니다.", code_position(src.file, tokens[i].line));
						}
					}
				}

				if (i == tokens.size())
				{
					throw compiler_error("예기치 않은 파일의 끝이 나타났습니다.", code_position(src.file, tokens[i - 1].line));
				}

				if (tokens[i].number == tmlparan)
				{
					do
					{
						i++;
						if (i == tokens.size())
						{
							throw compiler_error("예기치 않은 파일의 끝이 나타났습니다.", code_position(src.file, tokens[i - 1].line));
						}

						if IS_ACESS_TYPE(tokens[i].number)
						{
							switch (tokens[i].number)
							{
							case sym_internal: at = INTERNAL; break;
							case sym_private: at = PRIVATE; break;
							case sym_public: at = PUBLIC; break;
							case sym_protected:
								if (tokens[i + 1].number == sym_internal)
								{
									at = PROCTED_INTERNAL;
									i++;
								}
								else
								{
									at = PROTECTED;
								}
								break;
							}
						}
						else if (tokens[i].number == tident || IS_TYPE(tokens[i].number))
						{
							tn = tokens[i];

							i++;
							if (i == tokens.size())
							{
								throw compiler_error("예기치 않은 파일의 끝이 나타났습니다.", code_position(src.file, tokens[i - 1].line));
							}

							if (tokens[i].number == tident)
							{
								name = tokens[i].value;

								i++;
								if (i == tokens.size())
								{
									throw compiler_error("예기치 않은 파일의 끝이 나타났습니다.", code_position(src.file, tokens[i - 1].line));
								}

								if (tokens[i].number == tlparan) // function
								{
									Function func;
									func.at = at;
									func.tn = tn;
									func.name = name;

									if (i == tokens.size())
									{
										throw compiler_error("예기치 않은 파일의 끝이 나타났습니다.", code_position(src.file, tokens[i - 1].line));
									}

									while (tokens[i].number != trparan)
									{
										i++;
										tn = tokens[i];

										if (tokens[i].number == trparan)
										{
											if (tn.number != sym_void && tn.number != trparan)
											{
												throw compiler_error("올바르지 않은 인수입니다.", code_position(src.file, tokens[i].line));
											}

											break;
										}

										Var param;
										param.at = INTERNAL;
										param.tn = tn;
										param.name = tokens[i].value;

										i++;
										if (i == tokens.size())
										{
											throw compiler_error("예기치 않은 파일의 끝이 나타났습니다.", code_position(src.file, tokens[i - 1].line));
										}

										if (tokens[i].number == tequal)
										{
											i++;
											if (i == tokens.size())
											{
												throw compiler_error("예기치 않은 파일의 끝이 나타났습니다.", code_position(src.file, tokens[i - 1].line));
											}

											param.init = tokens[i];
										}
										else if (tokens[i].number == trparan)
										{
											func.params.push_back(param);
											break;
										}

										func.params.push_back(param);
										i++;
										if (i == tokens.size())
										{
											throw compiler_error("예기치 않은 파일의 끝이 나타났습니다.", code_position(src.file, tokens[i - 1].line));
										}
									}

									i++;
									if (tokens[i].number == tmlparan)
									{
										i++; mparan_cnt++;

										while (mparan_cnt > 0 && tokens[i].number != tmrparan)
										{
											if (tokens[i].number == tstring)
											{
												if (std::find(ctx.Strings.begin(), ctx.Strings.end(),
													tokens[i].value) == ctx.Strings.end())
												{
													ctx.Strings.push_back(tokens[i].value);
												}

												size_t idx = std::find(ctx.Strings.begin(), ctx.Strings.end(), tokens[i].value) - ctx.Strings.begin();
												tokens[i].value = std::to_string(idx);
											}

											if (tokens[i].number == tmlparan) mparan_cnt++;
											else if (tokens[i].number == tmrparan) mparan_cnt--;

											func.inner.push_back(tokens[i]);

											i++;
										}

										mparan_cnt--;
									}

									cls.funcs.push_back(func);
								}
								else if (tokens[i].number == tmlparan) // property
								{
									i++; mparan_cnt++;

									do
									{
										Function func;

										if (tokens[i].value == "get") // get
										{
											i++;

											func.at = at;
											func.tn = tn;
											func.name = name + "@@GET_get";

											if (tokens[i].number == tmlparan)
											{
												i++; mparan_cnt++;

												while (mparan_cnt > 1 && tokens[i].number != tmrparan)
												{
													if (tokens[i].number == tstring)
													{
														if (std::find(ctx.Strings.begin(), ctx.Strings.end(),
															tokens[i].value) == ctx.Strings.end())
														{
															ctx.Strings.push_back(tokens[i].value);
														}

														size_t idx = std::find(ctx.Strings.begin(), ctx.Strings.end(), tokens[i].value) - ctx.Strings.begin();
														tokens[i].value = std::to_string(idx);
													}

													if (tokens[i].number == tmlparan) mparan_cnt++;
													else if (tokens[i].number == tmrparan) mparan_cnt--;

													func.inner.push_back(tokens[i]);

													i++;
												}
											}
											else if (tokens[i].number == tsemicolon)
											{
												Var var;
												var.at = at;
												var.name = "__@@" + name;
												var.tn = tn;

												if (std::find(cls.vars.begin(), cls.vars.end(),
													var) == cls.vars.end())
												{
													cls.vars.push_back(var);
												}

												func.inner.push_back(
													Token(sym_return, tokens[i].line, "return"));

												func.inner.push_back(
													Token(tident, tokens[i].line, "__@@" + name));

												func.inner.push_back(
													Token(tsemicolon, tokens[i].line, ";"));
											}

											i++;
											cls.funcs.push_back(func);
										}
										else if (tokens[i].value == "set") // set
										{
											i++;

											func.tn = tn;
											func.name = name + "@@GET_set";
											func.params.push_back(Var(PRIVATE, tn, "value", Token()));

											if (tokens[i].number == tmlparan)
											{
												i++; mparan_cnt++;

												while (mparan_cnt > 1 && tokens[i].number != tmrparan)
												{
													if (tokens[i].number == tstring)
													{
														if (std::find(ctx.Strings.begin(), ctx.Strings.end(),
															tokens[i].value) == ctx.Strings.end())
														{
															ctx.Strings.push_back(tokens[i].value);
														}

														size_t idx = std::find(ctx.Strings.begin(), ctx.Strings.end(), tokens[i].value) - ctx.Strings.begin();
														tokens[i].value = std::to_string(idx);
													}

													if (tokens[i].number == tmlparan) mparan_cnt++;
													else if (tokens[i].number == tmrparan) mparan_cnt--;

													func.inner.push_back(tokens[i]);

													i++;
												}
											}
											else if (tokens[i].number == tsemicolon)
											{
												Var var;
												var.at = at;
												var.name = "__@@" + name;
												var.tn = tn;

												if (std::find(cls.vars.begin(), cls.vars.end(),
													var) == cls.vars.end())
												{
													cls.vars.push_back(var);
												}

												func.inner.push_back(
													Token(tident, tokens[i].line, "__@@" + name));

												func.inner.push_back(
													Token(tequal, tokens[i].line, "="));

												func.inner.push_back(
													Token(tident, tokens[i].line, "value"));

												func.inner.push_back(
													Token(tsemicolon, tokens[i].line, ";"));
											}

											i++;
											cls.funcs.push_back(func);
										}
									} while (mparan_cnt > 0 && tokens[i].number != tmrparan);

									mparan_cnt--;
								}
								else // var
								{
									Var var;

									while (1)
									{
										var.at = at;
										var.name = name;
										var.tn = tn;

										if (tokens[i].number == tequal)
										{
											i++;
											var.init = tokens[i];
											i++;
										}

										cls.vars.push_back(var);

										at = INTERNAL;

										if (tokens[i].number == tcomma)
											continue;
										else if (tokens[i].number == tsemicolon)
											break;
									}
								}
							}
							else if IS_KEYWORD(tokens[i].number)
							{
								throw compiler_error("키워드를 이름으로 할 수 없습니다.", code_position(src.file, tokens[i].line));
							}
							else
							{
								throw compiler_error("옳지 않은 이름입니다.", code_position(src.file, tokens[i].line));
							}
						}

					} while (!(mparan_cnt == 0 && tokens[i].number == tmrparan));
				}
				else
				{

				}

				ctx.Classes.push_back(cls);
			}
 		}
	}

	void SaveToFile(const std::string& filename)
	{
		std::ofstream file;

		file.open(filename, std::ios::out);
		file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

		file << "refer" << std::endl;

		for (const std::string& refer : ctx.refers)
		{
			file << "\"" << refer << "\"" << std::endl;
		}

		file << std::endl << "data" << std::endl;

		for (const std::string& data : ctx.Strings)
		{
			file << "\"" << data << "\"" << std::endl;
		}

		file << std::endl << "typeinfo" << std::endl;

		for (Class cls : ctx.Classes)
		{
			file << "class \"" << cls.name << "\", Access: ";

			file << AccessTypeStr[cls.at];

			file << ", Type: ";

			file << ClassTypeStr[cls.ct];

			file << std::endl;

			for (Var var : cls.vars)
			{
				file << "var \"" << var.name << "\", Access: ";
				file << AccessTypeStr[var.at];
				file << ", Type: ";
				file << var.tn.value;

				if (var.init.value != "")
					file << ", Init: " << var.init.value;

				file << std::endl;
			}

			for (Function func : cls.funcs)
			{
				file << "func \"" << func.name << "\", Access: ";
				file << AccessTypeStr[func.at];
				file << ", Ret-Type: ";
				file << func.tn.value;

				file << std::endl << "params" << std::endl;
				for (Var param : func.params)
				{
					file << "param \"" << param.name << "\", Type: ";
					file << param.tn.value;

					if (param.init.value != "")
						file << ", Init: " << param.init.value;

					file << std::endl;
				}

				file << "inner" << std::endl;
				for (Token token : func.inner)
				{
					file << token.value << " ";
				}

				file << std::endl;
			}
		}

		file.close();
	}

private:
	source src;
	compile_context& ctx;

	std::vector<Token> tokens;
};