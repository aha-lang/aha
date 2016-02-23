#pragma once

#include <ahabin/ahabindef.h>
#include <ahabin/AhaClass.h>
#include <ahabin/AhaClsMember.h>
#include <ahabin/AhaType.h>
#include <ahabin/AhaOpcode.h>

enum class ParseState
{
	Yet, Parsing, Completed
};

class Parser
{
private:
	ParseState m_psModuleName = ParseState::Yet;
	ParseState m_psRefer = ParseState::Yet;
	ParseState m_psNativeRefer = ParseState::Yet;
	ParseState m_psBody = ParseState::Yet;

	ParseState m_psClass = ParseState::Yet;
	ParseState m_psMember = ParseState::Yet;
	ParseState m_psOpcode = ParseState::Yet;

	aha::aha_u32 m_ModuleName;

	std::vector<std::wstring> m_strtbl;

	std::vector<aha::aha_u32> m_Refer;
	std::vector<aha::aha_u32> m_NativeRefer;

	std::vector<aha::AhaClass> m_ClassList;
	aha::AhaClass m_Class;

	std::vector<aha::AhaClsMember> m_MemberList;
	aha::AhaClsMember m_Member;

	std::vector<aha::AhaType> m_Params;
	std::vector<uint8_t> m_Opcode;

	typedef boost::tokenizer<boost::escaped_list_separator<wchar_t>,
		std::wstring::const_iterator, std::wstring>
		tokenizer;
	boost::escaped_list_separator<wchar_t> m_separator;

	typedef boost::tokenizer<boost::char_separator<wchar_t>,
		std::wstring::const_iterator, std::wstring>
		punct_tokenizer;
	boost::char_separator<wchar_t> m_PunctSeparator;

public:
	Parser();
	~Parser();

	/**
	 * @brief 줄 하나를 파싱합니다.
	 * @param[in] line 파싱할 줄입니다.
	 * @returns 파싱이 끝났으면 true이고, 끝나지 않았으면 false입니다.
	 */
	bool Line(std::wstring line);

	void Save(const char *file);

private:
	aha::aha_u32 AddOrGetStr(const std::wstring &str);

	void ParseClass(const std::wstring &line);
	void ParseMember(const std::wstring &line, const std::vector<std::wstring> &vttok);

	void ParseOpcode(const std::vector<std::wstring> &vttok);
	void ParseOpcode_callnative(const std::vector<std::wstring> &vttok);
	void ParseOpcode_NiladicOp(const std::vector<std::wstring> &vttok);
	void ParseOpcode_UnaryOp_uintptr(const std::vector<std::wstring> &vttok);
	void ParseOpcode_ldc_XXX(aha::AhaOpcode opcode, const std::vector<std::wstring> &vttok);
	void ParseOpcode_call(const std::vector<std::wstring> &vttok);
	void ParseOpcode_newobj(const std::vector<std::wstring> &vttok);

	aha::AhaVariable ParseType(aha::AhaType type, const std::wstring &str);

	static aha::AhaAccess StrToAhaAccess(const std::wstring &str);
	static aha::AhaClassType StrToAhaClassType(const std::wstring &str);
	static aha::AhaClsMemberType StrToAhaClsMemberType(const std::wstring &str);
	static aha::AhaClsMemberStorage StrToAhaClsMemberStorage(const std::wstring &str);
	aha::AhaType StrToAhaType(const std::wstring &str);
	static aha::AhaOpcode StrToAhaOpcode(const std::wstring &str);
	static aha::AhaNativeCallType StrToAhaNativeCallType(const std::wstring &str);
};

class ParseError
{
private:
	std::wstring m_str;
public:
	explicit ParseError(const std::wstring &str) : m_str(str) { }
	explicit ParseError(std::wstring &&str) : m_str(std::forward<std::wstring>(str)) { }
	const std::wstring what() const throw() { return m_str; }
};
