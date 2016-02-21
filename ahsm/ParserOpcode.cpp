#include "stdafx.h"
#include "Parser.h"

void Parser::ParseOpcode(const std::vector<std::wstring> &vttok)
{
	aha::AhaOpcode opcode = StrToAhaOpcode(vttok[0]);

	m_Opcode.push_back((uint8_t)opcode);

	switch (opcode)
	{
		// 선두 opcode
		case aha::AHA_OPCODE_locals:
			ParseOpcode_locals(vttok);
			break;
		case aha::AHA_OPCODE_native:
			ParseOpcode_native(vttok);
			break;

		// 무항 opcode
		case aha::AHA_OPCODE_add:
		case aha::AHA_OPCODE_sub:
		case aha::AHA_OPCODE_mul:
		case aha::AHA_OPCODE_div:
		case aha::AHA_OPCODE_or:
		case aha::AHA_OPCODE_and:
		case aha::AHA_OPCODE_xor:
		case aha::AHA_OPCODE_shl:
		case aha::AHA_OPCODE_shr:
		case aha::AHA_OPCODE_not:
		case aha::AHA_OPCODE_dup:
		case aha::AHA_OPCODE_ldthis:
		case aha::AHA_OPCODE_ret:
		case aha::AHA_OPCODE_conv_byte:
		case aha::AHA_OPCODE_conv_i8:
		case aha::AHA_OPCODE_conv_i16:
		case aha::AHA_OPCODE_conv_i32:
		case aha::AHA_OPCODE_conv_i64:
		case aha::AHA_OPCODE_conv_f32:
		case aha::AHA_OPCODE_conv_f64:
		case aha::AHA_OPCODE_conv_iptr:
		case aha::AHA_OPCODE_ldnull:
			ParseOpcode_NiladicOp(vttok);
			break;

		// 단항 opcode - uintptr_t를 인수로 받음
		case aha::AHA_OPCODE_ldloc:
		case aha::AHA_OPCODE_stloc:
		case aha::AHA_OPCODE_ldvar:
		case aha::AHA_OPCODE_stvar:
		case aha::AHA_OPCODE_ldstr:
		case aha::AHA_OPCODE_jmp:
		case aha::AHA_OPCODE_je:
		case aha::AHA_OPCODE_jne:
		case aha::AHA_OPCODE_jl:
		case aha::AHA_OPCODE_jle:
		case aha::AHA_OPCODE_jg:
		case aha::AHA_OPCODE_jge:
		case aha::AHA_OPCODE_ldpar:
		case aha::AHA_OPCODE_stpar:
			ParseOpcode_UnaryOp_uintptr(vttok);
			break;

		// ldc_XXX
		case aha::AHA_OPCODE_ldc_bool:
		case aha::AHA_OPCODE_ldc_byte:
		case aha::AHA_OPCODE_ldc_i8:
		case aha::AHA_OPCODE_ldc_i16:
		case aha::AHA_OPCODE_ldc_i32:
		case aha::AHA_OPCODE_ldc_i64:
		case aha::AHA_OPCODE_ldc_f32:
		case aha::AHA_OPCODE_ldc_f64:
			ParseOpcode_ldc_XXX(opcode, vttok);
			break;

		// call
		case aha::AHA_OPCODE_call:
			ParseOpcode_call(vttok);
			break;

		// newobj
		case aha::AHA_OPCODE_newobj:
			ParseOpcode_newobj(vttok);
			break;
	}
}

void Parser::ParseOpcode_locals(const std::vector<std::wstring> &vttok)
{
	if (m_Opcode.size() != 1)
		throw ParseError(L"'locals' must be at first of body of function");
	if (vttok.size() < 2)
		throw ParseError(L"param of 'locals' cannot be empty");

	aha::AhaType type;
	wchar_t *endptr;

	for (auto it = std::next(vttok.begin()); it != vttok.end(); it++)
	{
		type = StrToAhaType(*it);
		m_Opcode.push_back((uint8_t)type);

		if (!(type & 0x80000000))
		{
			errno = 0;
			unsigned long ul = wcstoul(it->c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' /* || ul >= m_strtbl.size() */)
				throw ParseError(L"there must be a primitive type name or an string index");

			m_Opcode.insert(m_Opcode.end(), (uint8_t *)&ul, (uint8_t *)&ul + sizeof(ul));
		}
	}
}

void Parser::ParseOpcode_native(const std::vector<std::wstring> &vttok)
{
	if (m_Opcode.size() != 1)
		throw ParseError(L"'native' must be at first of body of function");
	if (vttok.size() != 3)
		throw ParseError(L"'native' is binary opcode; it must have two terms");

	punct_tokenizer tk(vttok[2], m_PunctSeparator);
	std::vector<std::wstring> vttok2(tk.begin(), tk.end());
	wchar_t *endptr;

	if (vttok2.size() != 2)
		throw ParseError(L"the second param of 'native' must be: string indices of library and function");

	aha::AhaNativeCallType calltype = StrToAhaNativeCallType(vttok[1]);

	errno = 0;
	unsigned long lib = wcstoul(vttok2[0].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' /* || lib >= m_strtbl.size() */)
		throw ParseError(L"there must be an string index");

	errno = 0;
	unsigned long func = wcstoul(vttok2[1].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' /* || func >= m_strtbl.size() */)
		throw ParseError(L"there must be an string index");

	m_Opcode.push_back((uint8_t)calltype);
	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&lib, (uint8_t *)&lib + sizeof(lib));
	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&func, (uint8_t *)&func + sizeof(func));
}

void Parser::ParseOpcode_NiladicOp(const std::vector<std::wstring> &vttok)
{
	if (vttok.size() != 1)
		throw ParseError(L"'" + vttok[0] + L"' is niladic opcode; it cannot have any terms");
}

void Parser::ParseOpcode_UnaryOp_uintptr(const std::vector<std::wstring> &vttok)
{
	if (vttok.size() != 2)
		throw ParseError(L"'" + vttok[0] + L"' is unary opcode; it must have one term");

	wchar_t *endptr;
	errno = 0;
	unsigned long ul = wcstoul(vttok[1].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0')
		throw ParseError(L"there must be a uintptr literal");

	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&ul, (uint8_t *)&ul + sizeof(ul));
}

void Parser::ParseOpcode_ldc_XXX(aha::AhaOpcode opcode, const std::vector<std::wstring> &vttok)
{
	if (vttok.size() != 2)
		throw ParseError(L"'" + vttok[0] + L"' is unary opcode; it must have one term");

	aha::AhaType type;
	switch (opcode)
	{
		case aha::AHA_OPCODE_ldc_bool: type = aha::AHA_TYPE_BOOL; break;
		case aha::AHA_OPCODE_ldc_byte: type = aha::AHA_TYPE_UINT8; break;
		case aha::AHA_OPCODE_ldc_i8: type = aha::AHA_TYPE_INT8; break;
		case aha::AHA_OPCODE_ldc_i16: type = aha::AHA_TYPE_INT16; break;
		case aha::AHA_OPCODE_ldc_i32: type = aha::AHA_TYPE_INT32; break;
		case aha::AHA_OPCODE_ldc_i64: type = aha::AHA_TYPE_INT64; break;
		case aha::AHA_OPCODE_ldc_f32: type = aha::AHA_TYPE_FLOAT32; break;
		case aha::AHA_OPCODE_ldc_f64: type = aha::AHA_TYPE_FLOAT64; break;
	}

	size_t size = aha::AhaTypeGetSize(type);
	aha::AhaVariable var = ParseType(type, vttok[1]);
	m_Opcode.resize(m_Opcode.size() + size);
	void *des = m_Opcode.data() + m_Opcode.size() - size;
	memcpy(des, &var, size);
}

void Parser::ParseOpcode_call(const std::vector<std::wstring> &vttok)
{
	if (vttok.size() != 2)
		throw ParseError(L"'call' is unary opcode; it must have one term");

	punct_tokenizer tk(vttok[1], m_PunctSeparator);
	std::vector<std::wstring> vttok2(tk.begin(), tk.end());
	wchar_t *endptr;

	if (vttok2.size() != 3)
		throw ParseError(L"param of 'call' must be: string indices of library, class, and function");

	errno = 0;
	unsigned long lib = wcstoul(vttok2[0].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' /* || lib >= m_strtbl.size()*/)
		throw ParseError(L"there must be an string index");

	errno = 0;
	unsigned long cls = wcstoul(vttok2[1].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' /* || cls >= m_strtbl.size() */)
		throw ParseError(L"there must be an string index");

	errno = 0;
	unsigned long func = wcstoul(vttok2[2].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' /* || func >= m_strtbl.size() */)
		throw ParseError(L"there must be an string index");

	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&lib, (uint8_t *)&lib + sizeof(lib));
	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&cls, (uint8_t *)&cls + sizeof(cls));
	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&func, (uint8_t *)&func + sizeof(func));
}

void Parser::ParseOpcode_newobj(const std::vector<std::wstring> &vttok)
{
	if (vttok.size() != 2)
		throw ParseError(L"'newobj' is unary opcode; it must have one term");

	punct_tokenizer tk(vttok[1], m_PunctSeparator);
	std::vector<std::wstring> vttok2(tk.begin(), tk.end());
	wchar_t *endptr;

	if (vttok2.size() != 3)
		throw ParseError(L"param of 'newobj' must be: string indices of library, class and constructor function");

	errno = 0;
	unsigned long lib = wcstoul(vttok2[0].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' /* || lib >= m_strtbl.size() */)
		throw ParseError(L"there must be an string index");

	errno = 0;
	unsigned long cls = wcstoul(vttok2[1].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' /* || cls >= m_strtbl.size() */)
		throw ParseError(L"there must be an string index");

	errno = 0;
	unsigned long ctor = wcstoul(vttok2[2].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' /* || ctor >= m_strtbl.size() */)
		throw ParseError(L"there must be an string index");

	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&lib, (uint8_t *)&lib + sizeof(lib));
	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&cls, (uint8_t *)&cls + sizeof(cls));
	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&ctor, (uint8_t *)&ctor + sizeof(ctor));
}
