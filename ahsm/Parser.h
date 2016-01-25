#pragma once

#include "../AhaModuleLib/AMLHeader.h"

enum class ParseState
{
	Yet, Parsing, Completed
};

class Parser
{
private:
	ParseState m_psAttr = ParseState::Yet;
	ParseState m_psRefer = ParseState::Yet;
	ParseState m_psNativeRefer = ParseState::Yet;
	ParseState m_psStrData = ParseState::Yet;
	ParseState m_psTypeInfo = ParseState::Yet;

	std::vector<AhaAttribute *> m_Attr;
	std::vector<std::wstring> m_Refer;
	std::vector<std::wstring> m_NativeRefer;
	std::vector<std::wstring> m_StrData;
	ParseState m_psClass = ParseState::Yet;

	std::vector<AhaClass> m_TypeInfo;
	AhaClass m_Class;

	ParseState m_psMember = ParseState::Yet;
	std::vector<AhaMember> m_MemberList;
	AhaMember m_Member;

	ParseState m_psOpcode = ParseState::Yet;
	std::vector<uint8_t> m_Opcode;

	std::vector<std::wstring> m_AttrKind;

	std::vector<void *> m_MallocedMem;
	boost::pool<> m_MemPool;

	typedef boost::tokenizer<boost::char_separator<wchar_t>,
		std::wstring::const_iterator, std::wstring>
		cs_tokenizer;

	typedef cs_tokenizer tokenizer;
	boost::char_separator<wchar_t> m_separator;
	//boost::escaped_list_separator<wchar_t> m_separator;

	typedef cs_tokenizer punct_tokenizer;
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
	void ParseClass(const std::wstring &line);
	void ParseMember(const std::wstring &line, const std::vector<std::wstring> &vttok);

	void ParseOpcode(const std::vector<std::wstring> &vttok);
	void ParseOpcode_locals(const std::vector<std::wstring> &vttok);
	void ParseOpcode_native(const std::vector<std::wstring> &vttok);
	void ParseOpcode_NiladicOp(const std::vector<std::wstring> &vttok);
	void ParseOpcode_UnaryOp_uintptr(const std::vector<std::wstring> &vttok);
	void ParseOpcode_ldc_XXX(Opcode opcode, const std::vector<std::wstring> &vttok);
	void ParseOpcode_call(const std::vector<std::wstring> &vttok);
	void ParseOpcode_newobj(const std::vector<std::wstring> &vttok);

	const AhaChar *MallocStr(const std::wstring &str);

	void *ParseType(AhaTypeKind tkind, const std::wstring &str, size_t *psz);

	static AhaAccess StrToAhaAccess(const std::wstring &str);
	static AhaClassKind StrToAhaClassKind(const std::wstring &str);
	static AhaMemberKind StrToAhaMemberKind(const std::wstring &str);
	static AhaMemberContainKind StrToAhaMemberContainKind(const std::wstring &str);
	static AhaTypeKind StrToAhaTypeKind(const std::wstring &str);
	static Opcode StrToOpcode(const std::wstring &str);
	static NativeCallType StrToNativeCallType(const std::wstring &str);
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

class AMLError : public std::exception
{
private:
	Result m_rs;
	explicit AMLError(Result result) : m_rs(result) { }
public:
	static void Throw(Result rs)
	{
		if (rs == R_OUT_OF_MEMORY)
		{
			throw std::bad_alloc();
		}
		else
		{
			throw AMLError(rs);
		}
	}

	virtual const char *what() const throw() override;
};
