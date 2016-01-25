#include "stdafx.h"
#include "Parser.h"

AhaAccess Parser::StrToAhaAccess(const std::wstring &str)
{
	std::vector<std::wstring> vt = {
		L"public",
		L"protected",
		L"protected_internal",
		L"internal",
		L"private",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not an access modifier");
	return (AhaAccess)(it - vt.begin());
}

AhaClassKind Parser::StrToAhaClassKind(const std::wstring &str)
{
	std::vector<std::wstring> vt = {
		L"normal",
		L"abstract",
		L"sealed",
		L"static",
		L"interface",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a class modifier");
	return (AhaClassKind)(it - vt.begin());
}

AhaMemberKind Parser::StrToAhaMemberKind(const std::wstring &str)
{
	std::vector<std::wstring> vt = {
		L"func",
		L"var",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a member modifier");
	return (AhaMemberKind)(it - vt.begin());
}

AhaMemberContainKind Parser::StrToAhaMemberContainKind(const std::wstring &str)
{
	std::vector<std::wstring> vt = {
		L"classof",
		L"instof",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a member contain-kind modifier");
	return (AhaMemberContainKind)(it - vt.begin());
}

AhaTypeKind Parser::StrToAhaTypeKind(const std::wstring &str)
{
	std::vector<std::wstring> vt = {
		L"void",	// AHA_TYPE_VOID
		L"bool",	// AHA_TYPE_BOOL
		L"byte",	// AHA_TYPE_BYTE
		L"int8",	// AHA_TYPE_INT8
		L"int16",	// AHA_TYPE_INT16
		L"int32",	// AHA_TYPE_INT32
		L"int64",	// AHA_TYPE_INT64
		L"float32",	// AHA_TYPE_FLOAT32
		L"float64",	// AHA_TYPE_FLOAT64
		L"intptr"	// AHA_TYPE_INTPTR
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it != vt.end())
	{
		return (AhaTypeKind)(it - vt.begin());
	}
	else
	{
		return AHA_TYPE_OBJECT;
	}
}

Opcode Parser::StrToOpcode(const std::wstring &str)
{
	std::vector<std::wstring> vt = {
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
	return (Opcode)(it - vt.begin());
}

NativeCallType Parser::StrToNativeCallType(const std::wstring &str)
{
	std::vector<std::wstring> vt = {
		L"stdcall",
		L"cdecl",
	};
	auto it = std::find(vt.begin(), vt.end(), str);
	if (it == vt.end())
		throw ParseError(L"'" + str + L"' is not a native call type");
	return (NativeCallType)(it - vt.begin());
}
