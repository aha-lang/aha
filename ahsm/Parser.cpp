#include "stdafx.h"
#include "Parser.h"

static void trim(std::wstring &str);

Parser::Parser()
	: m_separator(L"\\", L" \f\n\r\t\v", L"\"")
	, m_PunctSeparator(LR"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)")
{

}

Parser::~Parser()
{
}

aha::aha_u32 Parser::AddOrGetStr(const std::wstring& str)
{
	auto it = std::find(m_strtbl.begin(), m_strtbl.end(), str);
	if (it != m_strtbl.end())
	{
		return (aha::aha_u32)(it - m_strtbl.begin());
	}
	else
	{
		m_strtbl.push_back(str);
		return m_strtbl.size() - 1;
	}
}

bool Parser::Line(std::wstring line)
{
	trim(line);

	// 빈 줄은 무시
	if (line.empty())
		return false;

	if (m_psModuleName == ParseState::Yet)
	{
		tokenizer tok(line, m_separator);
		std::vector<std::wstring> vttok(tok.begin(), tok.end());

		if (!(vttok.size() == 2 && vttok[0] == L".module"))
			throw ParseError(L"there must be '.module' directive with module name");

		m_ModuleName = AddOrGetStr(vttok[1]);
		m_psModuleName = ParseState::Completed;
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

			aha::aha_u32 idstr = AddOrGetStr(
				std::wstring(
					std::next(line.begin()), std::prev(line.end())
					));
			m_Refer.emplace_back(idstr);
		}
	}
	else if (m_psNativeRefer == ParseState::Yet)
	{
		if (line != L"nrefer")
			throw ParseError(L"there must be 'nrefer'");
		m_psNativeRefer = ParseState::Parsing;
	}
	else if (m_psNativeRefer == ParseState::Parsing)
	{
		if (line == L"endnrefer")
		{
			m_psNativeRefer = ParseState::Completed;
		}
		else
		{
			if (!(line.size() >= 2 && line.front() == L'"' && line.back() == L'"'))
				throw ParseError(L"there must be a string literal");

			aha::aha_u32 idstr = AddOrGetStr(
				std::wstring(
					std::next(line.begin()), std::prev(line.end())
					));
			m_NativeRefer.emplace_back(idstr);
		}
	}
	else if (m_psBody == ParseState::Yet)
	{
		if (line != L"body")
			throw ParseError(L"there must be 'body'");
		m_psBody = ParseState::Parsing;
	}
	else if (m_psBody == ParseState::Parsing)
	{
		// TODO
		if (line == L"endbody")
		{
			if (m_psClass == ParseState::Parsing)
				throw ParseError(L"unexpected end of body");

			m_psBody = ParseState::Completed;
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

		auto it = std::find_if(m_ClassList.begin(), m_ClassList.end(),
			[&](const aha::AhaClass &cls) {
				return m_strtbl[cls.GetRaw().name] == vttok[3];
			});
		if (it != m_ClassList.end())
			throw ParseError(L"class '" + vttok[3] + L"' is already exist in this module");

		m_Class.GetRaw().access = StrToAhaAccess(vttok[0]);
		m_Class.GetRaw().type = StrToAhaClassType(vttok[1]);
		m_Class.GetRaw().name = AddOrGetStr(vttok[3]);
		m_Class.GetRaw().base = 0xffffffff; // TODO
		m_Class.GetRaw().interfaces = 0xffffffff; // TODO

		m_psClass = ParseState::Parsing;
	}
	else if (m_psClass == ParseState::Parsing)
	{
		if (line == L"endclass")
		{
			if (m_psMember == ParseState::Parsing)
				throw ParseError(L"unexpected end of class");

			m_Class.GetMembers() = std::move(m_MemberList);
			m_ClassList.push_back(m_Class);

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
			[&](const aha::AhaClsMember &cls) {
				return m_strtbl[cls.GetRaw().name] == vttok[4];
			});
		if (it != m_MemberList.end())
			throw ParseError(L"member '" + vttok[3] + L"' is already exist in '" + m_strtbl[m_Class.GetRaw().name] + L"' class");

		m_Member.GetRaw().access = StrToAhaAccess(vttok[0]);
		m_Member.GetRaw().type = StrToAhaClsMemberType(vttok[1]);
		m_Member.GetRaw().storage = StrToAhaClsMemberStorage(vttok[2]);
		m_Member.GetRaw().variable.vartype = StrToAhaType(vttok[3]);
		m_Member.GetRaw().name = AddOrGetStr(vttok[4]);

		if (m_Member.GetRaw().variable.vartype == aha::AHA_TYPE_VOID && m_Member.GetRaw().type == aha::AHA_CLSMEM_TYPE_VAR)
			throw ParseError(L"member variable '" + vttok[4] + L"' cannot be void");

		m_Member.GetRaw().function.SizeOfOpcode = 0;

		m_psMember = ParseState::Parsing;
	}
	else if (m_psMember == ParseState::Parsing)
	{
		if (m_Member.GetRaw().function.SizeOfOpcode == 0)
		{
			if (!(vttok.size() >= 2 && vttok.front() == L"(" && vttok.back() == L")"))
				throw ParseError(L"there must be ( ) token");

			if (m_Member.GetRaw().type == aha::AHA_CLSMEM_TYPE_VAR)
			{
				if (vttok.size() != 3)
					throw ParseError(L"there must be variable init value");

				m_Member.GetRaw().variable.initial = ParseType(m_Member.GetRaw().variable.vartype, vttok[1]);

				m_MemberList.push_back(m_Member);
				m_psMember = ParseState::Yet;
			}
			else
			{
				unsigned count = 0;

				for (auto it = std::next(vttok.begin()); it != std::prev(vttok.end()); it++)
				{
					m_Params.push_back(StrToAhaType(*it));
					if (m_Params.back() == aha::AHA_TYPE_VOID)
					{
						throw ParseError(L"parameter of function cannot be 'void'");
					}
					else if (!(m_Params.back() & aha::AHA_TYPE_FLG_PRIMITIVE))
					{
						m_Params.push_back((aha::AhaType)AddOrGetStr(*it));
					}
					count++;
				}

				m_Member.GetRaw().function.SizeOfOpcode = 1;
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

					m_Member.GetParams() = m_Params;
					m_Member.GetOpcode() = m_Opcode;

					m_Opcode.clear();
					m_Params.clear();
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
static void trim(std::wstring &str)
{
	str.erase(str.begin(),
		std::find_if(str.begin(), str.end(), [](wchar_t ch) { return !iswspace(ch); })
		);

	if (!str.empty())
	{
		str.erase(
			std::find_if(str.rbegin(), str.rend(), [](wchar_t ch) { return !iswspace(ch); }).base(),
			str.end()
			);
	}
}
