#include "stdafx.h"
#include "Parser.h"

void Parser::ParseOpcode(const std::vector<std::wstring> &vttok)
{
	Opcode opcode = StrToOpcode(vttok[0]);

	m_Opcode.push_back((uint8_t)opcode);

	switch (opcode)
	{
		// 선두 opcode
		case AHA_OPCODE_locals:
			ParseOpcode_locals(vttok);
			break;
		case AHA_OPCODE_native:
			ParseOpcode_native(vttok);
			break;

		// 무항 opcode
		case AHA_OPCODE_add:
		case AHA_OPCODE_sub:
		case AHA_OPCODE_mul:
		case AHA_OPCODE_div:
		case AHA_OPCODE_or:
		case AHA_OPCODE_and:
		case AHA_OPCODE_xor:
		case AHA_OPCODE_shl:
		case AHA_OPCODE_shr:
		case AHA_OPCODE_not:
		case AHA_OPCODE_dup:
		case AHA_OPCODE_ldthis:
		case AHA_OPCODE_ret:
		case AHA_OPCODE_conv_byte:
		case AHA_OPCODE_conv_i8:
		case AHA_OPCODE_conv_i16:
		case AHA_OPCODE_conv_i32:
		case AHA_OPCODE_conv_i64:
		case AHA_OPCODE_conv_f32:
		case AHA_OPCODE_conv_f64:
		case AHA_OPCODE_conv_ip:
		case AHA_OPCODE_ldnull:
			ParseOpcode_NiladicOp(vttok);
			break;

		// 단항 opcode - uintptr_t를 인수로 받음
		case AHA_OPCODE_ldloc:
		case AHA_OPCODE_stloc:
		case AHA_OPCODE_ldvar:
		case AHA_OPCODE_stvar:
		case AHA_OPCODE_ldstr:
		case AHA_OPCODE_jmp:
		case AHA_OPCODE_je:
		case AHA_OPCODE_jne:
		case AHA_OPCODE_jl:
		case AHA_OPCODE_jle:
		case AHA_OPCODE_jg:
		case AHA_OPCODE_jge:
		case AHA_OPCODE_ldpar:
		case AHA_OPCODE_stpar:
			ParseOpcode_UnaryOp_uintptr(vttok);
			break;

		// ldc_XXX
		case AHA_OPCODE_ldc_bool:
		case AHA_OPCODE_ldc_byte:
		case AHA_OPCODE_ldc_i8:
		case AHA_OPCODE_ldc_i16:
		case AHA_OPCODE_ldc_i32:
		case AHA_OPCODE_ldc_i64:
		case AHA_OPCODE_ldc_f32:
		case AHA_OPCODE_ldc_f64:
			ParseOpcode_ldc_XXX(opcode, vttok);
			break;

		// call
		case AHA_OPCODE_call:
			ParseOpcode_call(vttok);
			break;

		// newobj
		case AHA_OPCODE_newobj:
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

	AhaTypeKind tkind;
	wchar_t *endptr;

	for (auto it = std::next(vttok.begin()); it != vttok.end(); it++)
	{
		tkind = StrToAhaTypeKind(*it);
		m_Opcode.push_back((uint8_t)tkind);

		if (tkind == AHA_TYPE_OBJECT)
		{
			errno = 0;
			unsigned long ul = wcstoul(it->c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || ul >= m_StrData.size())
				throw ParseError(L"there must be a primitive type name or an strdata index");

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
		throw ParseError(L"the second param of 'native' must be: strdata indices of library and function");

	NativeCallType calltype = StrToNativeCallType(vttok[1]);

	errno = 0;
	unsigned long lib = wcstoul(vttok2[0].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' || lib >= m_StrData.size())
		throw ParseError(L"there must be an strdata index");

	errno = 0;
	unsigned long func = wcstoul(vttok2[1].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' || func >= m_StrData.size())
		throw ParseError(L"there must be an strdata index");

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

void Parser::ParseOpcode_ldc_XXX(Opcode opcode, const std::vector<std::wstring> &vttok)
{
	if (vttok.size() != 2)
		throw ParseError(L"'" + vttok[0] + L"' is unary opcode; it must have one term");

	AhaTypeKind tkind;
	switch (opcode)
	{
		case AHA_OPCODE_ldc_bool: tkind = AHA_TYPE_BOOL; break;
		case AHA_OPCODE_ldc_byte: tkind = AHA_TYPE_BYTE; break;
		case AHA_OPCODE_ldc_i8: tkind = AHA_TYPE_INT8; break;
		case AHA_OPCODE_ldc_i16: tkind = AHA_TYPE_INT16; break;
		case AHA_OPCODE_ldc_i32: tkind = AHA_TYPE_INT32; break;
		case AHA_OPCODE_ldc_i64: tkind = AHA_TYPE_INT64; break;
		case AHA_OPCODE_ldc_f32: tkind = AHA_TYPE_FLOAT32; break;
		case AHA_OPCODE_ldc_f64: tkind = AHA_TYPE_FLOAT64; break;
	}

	size_t size;
	void *src = ParseType(tkind, vttok[1], &size);
	m_Opcode.resize(m_Opcode.size() + size);
	void *des = m_Opcode.data() + m_Opcode.size() - size;
	memcpy(des, src, size);
}

void Parser::ParseOpcode_call(const std::vector<std::wstring> &vttok)
{
	if (vttok.size() != 2)
		throw ParseError(L"'call' is unary opcode; it must have one term");

	punct_tokenizer tk(vttok[1], m_PunctSeparator);
	std::vector<std::wstring> vttok2(tk.begin(), tk.end());
	wchar_t *endptr;

	if (vttok2.size() != 3)
		throw ParseError(L"param of 'call' must be: strdata indices of library, class, and function");

	errno = 0;
	unsigned long lib = wcstoul(vttok2[0].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' || lib >= m_StrData.size())
		throw ParseError(L"there must be an strdata index");

	errno = 0;
	unsigned long cls = wcstoul(vttok2[1].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' || cls >= m_StrData.size())
		throw ParseError(L"there must be an strdata index");

	errno = 0;
	unsigned long func = wcstoul(vttok2[2].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' || func >= m_StrData.size())
		throw ParseError(L"there must be an strdata index");

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
		throw ParseError(L"param of 'newobj' must be: strdata indices of library, class and constructor function");

	errno = 0;
	unsigned long lib = wcstoul(vttok2[0].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' || lib >= m_StrData.size())
		throw ParseError(L"there must be an strdata index");

	errno = 0;
	unsigned long cls = wcstoul(vttok2[1].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' || cls >= m_StrData.size())
		throw ParseError(L"there must be an strdata index");

	errno = 0;
	unsigned long ctor = wcstoul(vttok2[2].c_str(), &endptr, 0);
	if (errno == ERANGE || *endptr != L'\0' || ctor >= m_StrData.size())
		throw ParseError(L"there must be an strdata index");

	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&lib, (uint8_t *)&lib + sizeof(lib));
	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&cls, (uint8_t *)&cls + sizeof(cls));
	m_Opcode.insert(m_Opcode.end(), (uint8_t *)&ctor, (uint8_t *)&ctor + sizeof(ctor));
}
