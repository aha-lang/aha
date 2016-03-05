#pragma once

#include <vector>

#include "AhaTokens.h"

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