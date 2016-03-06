#pragma once

#include "AhaCompiler.h"

class code_tree_generator
{
public:
	static void Generate(compile_context& ctx, const source& source)
	{
		for (auto& cls : ctx.Classes)
		{
			for (auto& func : cls.funcs)
			{
				std::vector<std::vector<Token>> lines;

				{
					bool is_block = false;

					std::vector<Token> line;
					for (auto it = func.inner.begin(); it != func.inner.end(); it++)
					{
						if (IS_ITERATOR((*it).number) || IS_CONTROL((*it).number))
						{
							line.push_back(*it);

							if ((*it).number == sym_else)
							{
								if ((*(it + 1)).number != sym_if)
								{
									is_block = true;
									lines.push_back(line);
									line.clear();

									continue;
								}
							}

							it++;
							if ((*it).number == tmlparan)
							{
								if(is_block)
								{
									do
									{
										it++;
										if (it == func.inner.end())
										{
											throw compiler_error("예기치 못한 파일의 끝이 나타났습니다.", code_position(source.file, (*(it - 1)).line));
										}

										line.push_back(*it);
									} while ((*it).number == tmrparan);

									line.push_back(*it);
									lines.push_back(line);
									line.clear();
								}
								else
								{
									throw compiler_error("알 수 없는 식별자 '{'", code_position(source.file, (*it).line));
								}
							}
							else
							{
								throw compiler_error("'('이 없습니다.", code_position(source.file, (*it).line));
							}
						}
						else if ((*it).number == tsemicolon)
						{
							lines.push_back(line);
							line.clear();
						}
						else
						{
							line.push_back(*it);
						}
					}
				}

				//////////////////////////////////////////////////////////////

				for (auto& line : lines)
				{
					for (auto it = line.begin(); it != line.end(); it++)
					{
						if IS_TYPE((*it).number)
						{
							if (it == line.end())
							{
								throw compiler_error("잘못된 변수 선언문.", code_position(source.file, (*it).line));
							}

							it++;
						}
						else if ((*it).number == sym_return)
						{
							code src;
							src.type = CodeType::RETURN;

							src.inner.GetRoot().tokens.push_back(*it);
							if (it != line.end())
							{
								it++;
								Tree::Node *child = new Tree::Node();
								child->tokens.push_back(*it);

								src.inner.GetRoot().AddChild(child);
							}

							func.Code.push_back(src);
						}
						else if ((*it).number == tident)
						{
							code src;

							Tree::Node *left = new Tree::Node();
							left->tokens.push_back(*(it));
							src.inner.GetRoot().AddChild(left);

							if (it != line.end())
							{
								it++;

								if IS_CALC((*it).number)
								{
									src.type = CodeType::CALC;

									src.inner.GetRoot().tokens.push_back(*it);
									if (it == line.end())
									{
										throw compiler_error("우변이 없습니다.", code_position(source.file, (*it).line));
									}

									if ((*(it + 1)).number == tequal)
									{
										it++;
										src.inner.GetRoot().tokens.push_back(*it);

										if (it == line.end())
										{
											throw compiler_error("우변이 없습니다.", code_position(source.file, (*it).line));
										}
									}

									it++;

									Tree::Node *right = new Tree::Node();
									for (; it != line.end(); it++)
									{
										right->tokens.push_back(*it);
									}
									src.inner.GetRoot().AddChild(right);

									func.Code.push_back(src);
									break;
								}
								else if ((*it).number == tequal)
								{
									src.type = CodeType::CALC;

									src.inner.GetRoot().tokens.push_back(*it);

									if (it == line.end())
									{
										throw compiler_error("우변이 없습니다.", code_position(source.file, (*it).line));
									}

									it++;

									Tree::Node *right = new Tree::Node();
									for (; it != line.end(); it++)
									{
										right->tokens.push_back(*it);
									}
									src.inner.GetRoot().AddChild(right);

									func.Code.push_back(src);
									break;
								}
								else if ((*it).number == tlparan)
								{
									src.type = CodeType::CALL_FUNC;

									if (it == line.end())
									{
										throw compiler_error("')'을 찾을 수 없습니다.", code_position(source.file, (*it).line));
									}

									it++;

									std::vector<Token> tokens;
									for (; it != line.end(); it++)
									{
										if ((*it).number == tcomma)
										{
											Tree::Node *arg = new Tree::Node();

											for (auto& tk : tokens)
											{
												arg->tokens.push_back(tk);
											}

											src.inner.GetRoot().AddChild(arg);
										}

										if ((*it).number == trparan)
											break;

										tokens.push_back(*it);
									}

									Tree::Node *arg = new Tree::Node();

									for (auto& tk : tokens)
									{
										arg->tokens.push_back(tk);
									}

									src.inner.GetRoot().AddChild(arg);

									if (line[line.size() - 1].number != trparan)
									{
										throw compiler_error("')'을 찾을 수 없습니다.", code_position(source.file, (*it).line));
									}

									func.Code.push_back(src);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
};