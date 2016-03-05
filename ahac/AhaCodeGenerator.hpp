#pragma once

#include "AhaCompiler.h"

class Tree
{
public:
	struct Node
	{
		Node()
		{

		}

		Node(const Node& Other)
			: token(Other.token), Parent(Other.Parent)
		{
			for (auto& it : Other.Childs)
				Childs.push_back(Other);
		}

		Node& operator=(const Node& Other)
		{
			token = Other.token;
			Parent = Other.Parent;

			for (auto& it : Other.Childs)
				Childs.push_back(Other);

			return *this;
		}

		Token token;

		void AddChild(Node node)
		{
			node.Parent = *this;
			Childs.push_back(node);
		}

		Node Parent;
		std::vector<Node> Childs;
	};

public:
	Tree()
	{

	}

	Node GetRoot()
	{
		return root;
	}

private:
	Node root;
};

enum CodeType
{
	CALL_FUNC,
	CALC,
	VAR
};

struct code
{
	CodeType type;

	Tree inner;
};

class code_generator
{
public:
	static void Generate(const compile_context& ctx)
	{
		for (auto& cls : ctx.Classes)
		{
			for (auto& func : cls.funcs)
			{
				std::vector<std::vector<Token>> lines;

				{
					std::vector<Token> line;
					for (auto& it : func.inner)
					{
						if (it.number == tsemicolon)
						{
							lines.push_back(line);
							line.clear();
						}
						else
						{
							line.push_back(it);
						}
					}
				}
			}
		}
	}
};