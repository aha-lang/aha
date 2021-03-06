#include "stdafx.h"
#include "Parser.h"

aha::AhaAccess Parser::StrToAhaAccess(const std::wstring &str)
{
	static std::vector<std::wstring> vt = {
		L"public",
		L"protected",
		L"protected_internal",
		L"internal",
		L"private",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not an access modifier");
	return (aha::AhaAccess)(it - vt.begin());
}

aha::AhaClassType Parser::StrToAhaClassType(const std::wstring &str)
{
	static std::vector<std::wstring> vt = {
		L"normal",
		L"abstract",
		L"sealed",
		L"static",
		L"interface",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a class modifier");
	return (aha::AhaClassType)(it - vt.begin());
}

aha::AhaClsMemberType Parser::StrToAhaClsMemberType(const std::wstring &str)
{
	static std::vector<std::wstring> vt = {
		L"func",
		L"var",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a member modifier");
	return (aha::AhaClsMemberType)(it - vt.begin());
}

aha::AhaClsMemberStorage Parser::StrToAhaClsMemberStorage(const std::wstring &str)
{
	static std::vector<std::wstring> vt = {
		L"classof",
		L"instof",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a member contain-kind modifier");
	return (aha::AhaClsMemberStorage)(it - vt.begin());
}

aha::AhaType Parser::StrToAhaType(const std::wstring &str)
{
	static std::vector<std::wstring> vt = {
		L"void",
		L"bool",
		L"int8",
		L"uint8",
		L"int16",
		L"uint16",
		L"int32",
		L"uint32",
		L"int64",
		L"uint64",
		L"float32",
		L"float64",
		L"intptr",
		L"uintptr",
		L"char",
	};

	std::vector<std::wstring>::iterator it;

	if (str.back() == '*')
		it = std::find(vt.begin(), vt.end(), str.substr(0, str.size() - 1));
	else
		it = std::find(vt.begin(), vt.end(), str);

	if (it != vt.end())
	{
		return (aha::AhaType)((unsigned)(it - vt.begin())
			| aha::AHA_TYPE_FLG_PRIMITIVE | (str.back() == '*' ? aha::AHA_TYPE_FLG_POINTER : 0));
	}
	else
	{
		return AddOrGetStr(str);
	}
}

aha::AhaOpcode Parser::StrToAhaOpcode(const std::wstring &str)
{
	static std::vector<std::wstring> vt = {
		L"add",
		L"sub",
		L"mul",
		L"div",
		L"or",
		L"and",
		L"xor",
		L"shl",
		L"shr",
		L"not",
		L"dup",
		L"ldloc",
		L"stloc",
		L"ldvar",
		L"stvar",
		L"ldstr",
		L"ldc_bool",
		L"ldc_byte",
		L"ldc_i8",
		L"ldc_i16",
		L"ldc_i32",
		L"ldc_i64",
		L"ldc_f32",
		L"ldc_f64",
		L"jmp",
		L"call",
		L"ret",
		L"locals",
		L"je",
		L"jne",
		L"jl",
		L"jle",
		L"jg",
		L"jge",
		L"conv_byte",
		L"conv_i8",
		L"conv_i16",
		L"conv_i32",
		L"conv_i64",
		L"conv_f32",
		L"conv_f64",
		L"conv_ip",
		L"native",
		L"newobj",
		L"ldnull",
		L"ldpar",
		L"stpar",
		L"ldthis"
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a opcode");
	return (aha::AhaOpcode)(it - vt.begin());
}

aha::AhaNativeCallType Parser::StrToAhaNativeCallType(const std::wstring &str)
{
	static std::vector<std::wstring> vt = {
		L"stdcall",
		L"cdecl",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a native call type");
	return (aha::AhaNativeCallType)(it - vt.begin());
}
