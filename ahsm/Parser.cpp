#include "stdafx.h"
#include "Parser.h"

static void trim(std::wstring &str);

Parser::Parser()
	: m_AttrKind({ L"module" })
	, m_MemPool(sizeof(uint32_t))
	, m_separator(L" \f\n\r\t\v")
	//, m_separator(L"\\", L" \f\n\r\t\v", L"\"")
	, m_PunctSeparator(LR"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)")
{

}

Parser::~Parser()
{
	for (void *ptr : m_MallocedMem)
	{
		free(ptr);
	}
}

bool Parser::Line(std::wstring line)
{
	trim(line);

	// 빈 줄과 주석은 무시
	if (line.empty())
		return false;
	if (line.front() == L'#')
		return false;

	if (m_psAttr == ParseState::Yet)
	{
		if (line != L"attribute")
			throw ParseError(L"there must be 'attribute'");
		m_psAttr = ParseState::Parsing;
	}
	else if (m_psAttr == ParseState::Parsing)
	{
		if (line == L"endattr")
		{
			m_psAttr = ParseState::Completed;
		}
		else
		{
			if (!(line.size() >= 5 && line.front() == L'.'))
				throw ParseError(L"there must be a dot(.)");

			auto it_dot = std::next(line.begin());
			auto it = std::find_if(
				it_dot, line.end(), [](wchar_t ch){ return iswspace(ch); });
			if (it == line.end())
				throw ParseError(L"unexpected end of line in attr");

			std::wstring strAttrKind(it_dot, it);
			auto found = std::find(m_AttrKind.begin(), m_AttrKind.end(), strAttrKind);
			if (found == m_AttrKind.end())
				throw ParseError(L"unknown attribute keyword");

			it = std::find_if(
				it, line.end(), [](wchar_t ch){ return ch == L'"'; });
			if (it == line.end())
				throw ParseError(L"there must be a string literal");

			if (!(line.end() - it >= 1 && line.back() == L'"'))
				throw ParseError(L"there must be a string literal");

			AhaAttributeKind kind = (AhaAttributeKind)(found - m_AttrKind.begin());
			if (kind == MODULE_ATTRIBUTE)
			{
				AhaModuleAttr *pAttr;
				Result rs = CreateAhaModuleAttr(
					std::wstring(std::next(it), std::prev(line.end())).c_str(), &pAttr
					);
				if (RESULT_FAIL(rs))
					AMLError::Throw(rs);
				m_MallocedMem.push_back(pAttr);
				m_Attr.push_back((AhaAttribute *)pAttr);
			}
			else
			{
				throw ParseError(L"unknown attribute : '" + strAttrKind + L"'");
			}
		}
	}
	else if (m_psRefer == ParseState::Yet)
	{
		if (line != L"refer")
			throw ParseError(L"there must be 'refer'");
		m_psRefer = ParseState::Parsing;
	}
	else if (m_psRefer == ParseState::Parsing)
	{
		if (line == L"endrefer")
		{
			m_psRefer = ParseState::Completed;
		}
		else
		{
			if (!(line.size() >= 2 && line.front() == L'"' && line.back() == L'"'))
				throw ParseError(L"there must be a string literal");
			m_Refer.emplace_back(std::next(line.begin()), std::prev(line.end()));
		}
	}
	else if (m_psNativeRefer == ParseState::Yet)
	{
		if (line != L"nativerefer")
			throw ParseError(L"there must be 'nativerefer'");
		m_psNativeRefer = ParseState::Parsing;
	}
	else if (m_psNativeRefer == ParseState::Parsing)
	{
		if (line == L"endnativerefer")
		{
			m_psNativeRefer = ParseState::Completed;
		}
		else
		{
			if (!(line.size() >= 2 && line.front() == L'"' && line.back() == L'"'))
				throw ParseError(L"there must be a string literal");
			m_NativeRefer.emplace_back(std::next(line.begin()), std::prev(line.end()));
		}
	}
	else if (m_psStrData == ParseState::Yet)
	{
		if (line != L"data")
			throw ParseError(L"there must be 'data'");
		m_psStrData = ParseState::Parsing;
	}
	else if (m_psStrData == ParseState::Parsing)
	{
		if (line == L"enddata")
		{
			m_psStrData = ParseState::Completed;
		}
		else
		{
			if (!(line.size() >= 2 && line.front() == L'"' && line.back() == L'"'))
				throw ParseError(L"there must be a string literal");
			m_StrData.emplace_back(std::next(line.begin()), std::prev(line.end()));
		}
	}
	else if (m_psTypeInfo == ParseState::Yet)
	{
		if (line != L"typeinfo")
			throw ParseError(L"there must be 'typeinfo'");
		m_psTypeInfo = ParseState::Parsing;
	}
	else if (m_psTypeInfo == ParseState::Parsing)
	{
		// TODO!
		if (line == L"endtype")
		{
			if (m_psClass == ParseState::Parsing)
				throw ParseError(L"unexpected end of typeinfo");

			m_psTypeInfo = ParseState::Completed;
		}
		else
		{
			ParseClass(line);
		}
	}
	else
	{
		if (line != L"end")
			throw ParseError(L"there must be 'end'");
		return true;
	}

	return false;
}

void Parser::ParseClass(const std::wstring &line)
{
	tokenizer tok(line, m_separator);
	std::vector<std::wstring> vttok(tok.begin(), tok.end());

	if (m_psClass == ParseState::Yet)
	{
		if (!(vttok.size() == 4 && vttok[2] == L"class"))
			throw ParseError(L"there must be a declaration of class");

		auto it = std::find_if(m_TypeInfo.begin(), m_TypeInfo.end(),
			[&vttok](const AhaClass &cls) { return cls.name == vttok[3]; });
		if (it != m_TypeInfo.end())
			throw ParseError(L"class '" + vttok[3] + L"' is already exist in this module");

		m_Class.access = StrToAhaAccess(vttok[0]);
		m_Class.kind = StrToAhaClassKind(vttok[1]);
		m_Class.name = MallocStr(vttok[3]);
		m_Class.base = NULL; // TODO
		m_Class.interfaces = NULL; // TODO

		m_psClass = ParseState::Parsing;
	}
	else if (m_psClass == ParseState::Parsing)
	{
		if (line == L"endclass")
		{
			if (m_psMember == ParseState::Parsing)
				throw ParseError(L"unexpected end of class");

			Result rs = CreateAhaMemberTable(
				m_MemberList.data(), m_MemberList.size(), &m_Class.members, &m_Class.SizeOfMembers);
			if (RESULT_FAIL(rs))
				AMLError::Throw(rs);
			m_MallocedMem.push_back(m_Class.members);

			m_TypeInfo.push_back(m_Class);

			m_MemberList.clear();

			// 반복을 위해 completed 대신 yet을 사용
			m_psClass = ParseState::Yet;
			return;
		}

		ParseMember(line, vttok);
	}
}

void Parser::ParseMember(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	if (m_psMember == ParseState::Yet)
	{
		if (!(vttok.size() == 5))
			throw ParseError(L"there must be a declaration of member");

		auto it = std::find_if(m_MemberList.begin(), m_MemberList.end(),
			[&vttok](const AhaMember &cls) { return cls.name == vttok[4]; });
		if (it != m_MemberList.end())
			throw ParseError(L"member '" + vttok[3] + L"' is already exist in '" + m_Class.name + L"' class");

		m_Member.access = StrToAhaAccess(vttok[0]);
		m_Member.kind = StrToAhaMemberKind(vttok[1]);
		m_Member.ContainKind = StrToAhaMemberContainKind(vttok[2]);
		m_Member.rettype.kind = StrToAhaTypeKind(vttok[3]);
		if (m_Member.rettype.kind == AHA_TYPE_OBJECT)
			m_Member.TypeName = MallocStr(vttok[3]);
		else
			m_Member.TypeName = NULL;
		m_Member.name = MallocStr(vttok[4]);

		if (m_Member.rettype.kind == AHA_TYPE_VOID && m_Member.kind == AHA_MEMBER_VAR)
			throw ParseError(L"member variables '" + vttok[4] + L"' cannot be void");

		m_Member.params = NULL;
		m_Member.TypeName = NULL;
		m_Member.CountOfParams = 0;
		m_Member.SizeOfOpcode = 0;

		m_psMember = ParseState::Parsing;
	}
	else if (m_psMember == ParseState::Parsing)
	{
		if (m_Member.SizeOfOpcode == 0)
		{
			if (!(vttok.size() >= 2 && vttok.front() == L"(" && vttok.back() == L")"))
				throw ParseError(L"there must be ( ) token");

			if (m_Member.kind == AHA_MEMBER_VAR)
			{
				if (vttok.size() != 3)
					throw ParseError(L"there must be variables init value");

				m_Member.opcode = (uint8_t *)ParseType(m_Member.rettype.kind,
					vttok[1], &m_Member.SizeOfOpcode);

				m_MemberList.push_back(m_Member);
				m_psMember = ParseState::Yet;
			}
			else
			{
				std::vector<uint32_t> vt;
				unsigned count = 0;
				void *ptr;

				for (auto it = std::next(vttok.begin()); it != std::prev(vttok.end()); it++)
				{
					vt.push_back((uint32_t)StrToAhaTypeKind(*it));
					if (vt.back() == AHA_TYPE_VOID)
					{
						throw ParseError(L"parameter of function cannot be 'void'");
					}
					else if (vt.back() == AHA_TYPE_OBJECT)
					{
						vt.push_back((uint32_t)MallocStr(*it));
					}
					count++;
				}

				if (!vt.empty())
				{
					ptr = malloc(vt.size() * sizeof(uint32_t));
					if (ptr == NULL)
						throw std::bad_alloc();

					memcpy(ptr, vt.data(), vt.size() * sizeof(uint32_t));
					m_MallocedMem.push_back(ptr);
					m_Member.params = (AhaType *)ptr;
					m_Member.CountOfParams = count;
				}
				m_Member.SizeOfOpcode = 1;
			}
		}
		else
		{
			if (m_psOpcode == ParseState::Yet)
			{
				if (!(vttok.size() == 1 && vttok[0] == L"{"))
					throw ParseError(L"there must be body of function");
				m_psOpcode = ParseState::Parsing;
			}
			else
			{
				if (vttok.size() == 1 && vttok[0] == L"}")
				{
					if (m_Opcode.empty())
						throw ParseError(L"body of function cannnot be empty");

					void *ptr = malloc(m_Opcode.size());
					memcpy(ptr, m_Opcode.data(), m_Opcode.size());
					m_MallocedMem.push_back(ptr);

					m_Member.opcode = (uint8_t *)ptr;
					m_Member.SizeOfOpcode = m_Opcode.size();

					m_Opcode.clear();
					m_MemberList.push_back(m_Member);

					m_psOpcode = ParseState::Yet;
					m_psMember = ParseState::Yet;
				}
				else
				{
					ParseOpcode(vttok);
				}
			}
		}
	}
}


// utility

const AhaChar *Parser::MallocStr(const std::wstring &str)
{
	AhaChar *ret = (AhaChar *)calloc(str.size() + 1, sizeof(AhaChar));
	if (ret == NULL)
		throw std::bad_alloc();
	memcpy(ret, str.c_str(), (str.size() + 1) * sizeof(wchar_t));

	try
	{
		m_MallocedMem.push_back(ret);
		return ret;
	}
	catch (...)
	{
		free(ret);
		throw;
	}
}

static void trim(std::wstring &str)
{
	str.erase(str.begin(),
		std::find_if(str.begin(), str.end(), [](wchar_t ch) { return !iswspace(ch); })
		);

	if (!str.empty())
	{
		str.erase(str.rbegin().base(),
			std::find_if(str.rbegin(), str.rend(), [](wchar_t ch) { return !iswspace(ch); }).base()
			);
	}
}

const char *AMLError::what() const throw()
{
	static const char *tbl[] = {
		"R_SUCCESS",
		"R_PROCESSED_PARTIAL",
		"R_FUNCTION_RETURN",
		"R_FAIL",
		"R_OUT_OF_MEMORY",
		"R_STACK_OVERFLOW",
		"R_STACK_UNDERFLOW",
		"R_INVALID_OPERATION",
		"R_INVALID_CAST",
		"R_BAD_REFER",
		"R_BAD_NATIVE_REFER"
	};
	return tbl[m_rs];
}
