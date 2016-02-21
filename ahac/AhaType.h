#pragma once

#include <string>
#include <vector>

#include "AhaToken.h"

enum eAccessType
{
	PUBLIC,
	PRIVATE,
	PROTECTED,
	PROTECTED_INTERNAL,
	INTERNAL
};

extern const wchar_t* AccessTypeStr[];

enum eClassType
{
	ABSTRACT,
	SEALED,
	STATIC,
	INTERFACE,
	NORMAL
};

extern const wchar_t* ClassTypeStr[];

struct sVar
{
	eAccessType at;
	std::wstring tn;

	std::wstring name;

	std::wstring init;

	sVar() { }
	sVar(eAccessType _at, const std::wstring& _tn, const std::wstring& _name, const std::wstring& _init)
		: at(_at), tn(_tn), name(_name), init(_init)
	{
	}

	bool operator==(const sVar& var) const
	{
		return ((at == var.at) && (tn == var.tn)
			&& (name == var.name) && (init == var.init));
	}

	bool operator!=(const sVar& var) const
	{
		return !(*this == var);
	}
};

struct sFunction
{
	eAccessType at;
	std::wstring tn;

	std::wstring name;

	std::vector<sVar> params;
	std::vector<TokenType> inner;

	bool operator==(const sFunction& func) const
	{
		return ((at == func.at) && (tn == func.tn)
			&& (name == func.name) && (params == func.params)
			&& (inner == func.inner));
	}

	bool operator!=(const sFunction& func) const
	{
		return !(*this == func);
	}
};

struct sClass
{
	eAccessType at;
	eClassType ct;

	std::vector<sVar> vars;
	std::vector<sFunction> funcs;

	std::wstring name;

	bool operator==(const sClass& cls) const
	{
		return ((at == cls.at) && (ct == cls.ct)
			&& (name == cls.name) && (vars == cls.vars)
			&& (funcs == cls.funcs));
	}

	bool operator!=(const sClass& cls) const
	{
		return !(*this == cls);
	}
};