#pragma once

#include "AhaCompiler.h"

class code_tree_generator
{
private:
	static void _parseBlock(const std::vector<Token>& block, std::vector<code>& codes, const source& source)
	{
		std::vector<std::vector<Token>> lines;

		{
			std::vector<Token> line;
			for (auto it = block.begin(); it != block.end(); it++)
			{
				if (IS_ITERATOR((*it).number) || IS_CONTROL((*it).number))
				{
					line.push_back(*it);

					bool else_if = false;

					if ((*it).number == sym_else)
					{
						if ((*(it + 1)).number == sym_if)
						{
							it++;
							line.push_back(*it);

							else_if = true;
						}
					}

					if (else_if || (line[0].number == sym_if) || (IS_ITERATOR(line[0].number) && line[0].number != sym_do))
					{
						it++;
						if ((*it).number == tlparan)
						{
							line.push_back(*it);

							if (it == block.end())
							{
								throw compiler_error("잘못된 " + ((else_if) ? lines[lines.size() - 1][1].value : lines[lines.size() - 1][0].value) + "문입니다.",
									code_position(source.file, (*(it - 1)).line));
							}

							do
							{
								it++;
								if (it == block.end())
								{
									throw compiler_error("예기치 못한 파일의 끝이 나타났습니다.", code_position(source.file, (*(it - 1)).line));
								}

								line.push_back(*it);
							} while ((*it).number != trparan);

							else_if = false;
						}
						else
						{
							throw compiler_error("잘못된 " + ((else_if) ? line[1].value : line[0].value) + "문입니다.",
								code_position(source.file, (*(it - 1)).line));
						}
					}

					lines.push_back(line);
					line.clear();

					it++;
					if ((*it).number == tmlparan)
					{
						do
						{
							it++;
							if (it == block.end())
							{
								throw compiler_error("예기치 못한 파일의 끝이 나타났습니다.", code_position(source.file, (*(it - 1)).line));
							}

							line.push_back(*it);
						} while ((*it).number != tmrparan);

						line.push_back(*it);
						lines.push_back(line);
						line.clear();
					}
					else
					{
						throw compiler_error("'{'이 없습니다.", code_position(source.file, (*it).line));
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

		for (size_t idx = 0; idx < lines.size(); idx++)
		{
			auto& line = lines[idx];

			for (auto it = line.begin(); it != line.end(); it++)
			{
				if (IS_ITERATOR((*it).number) || IS_CONTROL((*it).number))
				{
					bool else_if = false;

					code src;
					src.type = CodeType::BLOCK;

					if ((*it).number == sym_else)
					{
						if ((*(it + 1)).number == sym_if)
						{
							it++;

							else_if = true;
						}
					}

					if (else_if || (line[0].number == sym_if) || (IS_ITERATOR(line[0].number) && line[0].number != sym_do))
					{
						it++;
						Tree::Node *left = new Tree::Node();
						if (else_if)
						{
							left->tokens.push_back(line[0]);
							left->tokens.push_back(line[1]);
						}
						else
						{
							left->tokens.push_back(line[0]);
						}
						src.inner.GetRoot().AddChild(left);

						it++;
						Tree::Node *middle = new Tree::Node();
						while ((*it).number != trparan)
						{
							middle->tokens.push_back(*it);
							it++;
						}
						src.inner.GetRoot().AddChild(middle);
					}

					Tree::Node *right = new Tree::Node();
					idx++;
					for (auto& t : lines[idx])
					{
						right->tokens.push_back(t);
					}
					src.inner.GetRoot().AddChild(right);

					codes.push_back(src);

					break;
				}

				if ((*it).number == sym_return)
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

					codes.push_back(src);
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

							codes.push_back(src);
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

							codes.push_back(src);
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

							codes.push_back(src);
							break;
						}
					}
				}

				if ((*it).number == tident || IS_TYPE((*it).number))
				{
					if (it == line.end())
					{
						throw compiler_error("옳지 않은 변수 선언문 입니다.", code_position(source.file, (*it).line));
					}

					it++;

					if ((*it).number == tident)
					{
						code src;
						src.type = CodeType::VAR;

						Tree::Node *type = new Tree::Node();
						type->tokens.push_back(*it);

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

							tokens.push_back(*it);
						}

						Tree::Node *arg = new Tree::Node();

						for (auto& tk : tokens)
						{
							arg->tokens.push_back(tk);
						}

						src.inner.GetRoot().AddChild(arg);

						codes.push_back(src);

						break;
					}
					else
					{
						throw compiler_error("옳지 않은 변수 명입니다.", code_position(source.file, (*it).line));
					}
				}
			}
		}
	}

public:
	static void Generate(compile_context& ctx, const source& source)
	{
		for (auto& cls : ctx.Classes)
		{
			for (auto& func : cls.funcs)
			{
				_parseBlock(func.inner, func.Code, source);
			}
		}
	}
};