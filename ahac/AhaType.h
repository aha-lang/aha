#pragma once

#include <vector>

#include "AhaTokens.h"

class Tree
{
public:
	class Node
	{
	public:
		Node(Node *parent = nullptr) : Parent(nullptr)
		{
			if (parent)
				parent->AddChild(this);
		}

		~Node()
		{
			RemoveAllChild();
		}

		void SetParent(Node *parent)
		{
			Remove();

			Parent = parent;

			if (Parent)
				Parent->AddChild(this);
		}

		Node* GetParent() const
		{
			return Parent;
		}

		void AddChild(Node *child)
		{
			if (child && (child != this))
			{
				Children.push_back(child);
				child->Parent = this;
			}
		}

		bool RemoveChild(Node *child)
		{
			auto it = std::find(Children.begin(), Children.end(), child);

			if (it != Children.end())
			{
				(*it)->Parent = nullptr;
				Children.erase(it);
				return true;
			}

			return false;
		}

		void RemoveAllChild()
		{
			for (auto& it : Children)
			{
				it->Parent = nullptr;
			}

			Children.clear();
		}

		void AllDelete()
		{
			Parent = nullptr;

			for (auto& it : Children)
			{
				it->AllDelete();
				delete it;
			}

			Children.clear();
		}

		void Remove()
		{
			if (Parent)
				Parent->RemoveChild(this);
		}

	public:
		std::vector<Token> tokens;

		Node *Parent;
		std::vector<Node *> Children;
	};

public:
	Tree() : Root(Node(nullptr)) { }

	void AllDelete()
	{
		Root.AllDelete();
	}

	Node& GetRoot()
	{
		return Root;
	}

private:
	Node Root;
};

enum class CodeType
{
	CALL_FUNC,
	CALC,
	VAR,
	RETURN
};

struct code
{
	code() { }
	code(CodeType _type, Tree _inner) : type(_type), inner(_inner) { }
	code(const code& Other) : type(Other.type), inner(Other.inner) { }

	code& operator=(const code& Other)
	{
		type = Other.type;
		inner = Other.inner;

		return *this;
	}

	CodeType type;

	Tree inner;
};

enum AccessType
{
	INTERNAL,
	PRIVATE,
	PUBLIC,
	PROTECTED,
	PROCTED_INTERNAL
};

const std::string AccessTypeStr[] = {
	"internal",
	"private",
	"public",
	"protected",
	"protected internal"
};

enum ClassType
{
	NORMAL,
	ABSTRACT,
	SEALED,
	STATIC,
	INTERFACE,
};

const std::string ClassTypeStr[] = {
	"",
	"abstract",
	"sealed",
	"static",
	"interface"
};

struct Var
{
	AccessType at;
	Token tn;

	std::string name;

	Token init;

	Var() { }
	Var(AccessType _at, const Token& _tn, const std::string& _name, const Token& _init)
		: at(_at), tn(_tn), name(_name), init(_init)
	{
	}

	bool operator==(const Var& var) const
	{
		return ((at == var.at) && (tn == var.tn)
			&& (name == var.name) && (init == var.init));
	}

	bool operator!=(const Var& var) const
	{
		return !(*this == var);
	}
};

struct Function
{
	AccessType at;
	Token tn;

	std::string name;

	std::vector<Var> params;
	std::vector<Token> inner;
	std::vector<code> Code;

	bool operator==(const Function& func) const
	{
		return ((at == func.at) && (tn == func.tn)
			&& (name == func.name) && (params == func.params)
			&& (inner == func.inner));
	}

	bool operator!=(const Function& func) const
	{
		return !(*this == func);
	}
};

struct Class
{
	AccessType at;
	ClassType ct;

	std::vector<std::string> parents;

	std::vector<Var> vars;
	std::vector<Function> funcs;

	std::string name;

	bool operator==(const Class& cls) const
	{
		return ((at == cls.at) && (ct == cls.ct)
			&& (name == cls.name) && (vars == cls.vars)
			&& (funcs == cls.funcs));
	}

	bool operator!=(const Class& cls) const
	{
		return !(*this == cls);
	}
};