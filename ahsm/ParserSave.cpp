#include "stdafx.h"
#include "Parser.h"

void Parser::Save(const char *file)
{
	AhaModule module;
	AhaModuleHeader *ModuleTable;
	size_t szTable;

	std::vector<const wchar_t *> vtstr;

	Result rs;

	// attr
	if (!m_Attr.empty())
	{
		rs = CreateAhaAttributeTable(m_Attr.data(), m_Attr.size(),
			&module.Attr, &module.header.SizeOfAttr);
		if (RESULT_FAIL(rs))
			AMLError::Throw(rs);
		m_MallocedMem.push_back(module.Attr);
	}
	else
	{
		module.Attr = NULL;
		module.header.SizeOfAttr = 0;
	}

	// refer
	if (!m_Refer.empty())
	{
		std::transform(m_Refer.begin(), m_Refer.end(), std::back_inserter(vtstr),
			[](const std::wstring &str) { return str.c_str(); });
		rs = CreateAhaReferTable(vtstr.data(), vtstr.size(),
			&module.Refer, &module.header.SizeOfRefer);
		if (RESULT_FAIL(rs))
			AMLError::Throw(rs);
		m_MallocedMem.push_back(module.Refer);
		vtstr.clear();
	}
	else
	{
		module.Refer = NULL;
		module.header.SizeOfRefer = 0;
	}

	// nativerefer
	if (!m_NativeRefer.empty())
	{
		std::transform(m_NativeRefer.begin(), m_NativeRefer.end(), std::back_inserter(vtstr),
			[](const std::wstring &str) { return str.c_str(); });
		rs = CreateAhaNativeReferTable(vtstr.data(), vtstr.size(),
			&module.NativeRefer, &module.header.SizeOfNativeRefer);
		if (RESULT_FAIL(rs))
			AMLError::Throw(rs);
		m_MallocedMem.push_back(module.NativeRefer);
		vtstr.clear();
	}
	else
	{
		module.NativeRefer = NULL;
		module.header.SizeOfNativeRefer = 0;
	}

	// strdata
	if (!m_StrData.empty())
	{
		std::transform(m_StrData.begin(), m_StrData.end(), std::back_inserter(vtstr),
			[](const std::wstring &str) { return str.c_str(); });
		rs = CreateAhaStrDataTable(vtstr.data(), vtstr.size(),
			&module.StrData, &module.header.SizeOfStrData);
		if (RESULT_FAIL(rs))
			AMLError::Throw(rs);
		m_MallocedMem.push_back(module.StrData);
		// vtstr.clear();
	}
	else
	{
		module.StrData = NULL;
		module.header.SizeOfStrData = 0;
	}

	// typeinfo
	if (!m_TypeInfo.empty())
	{
		rs = CreateAhaTypeInfoTable(m_TypeInfo.data(), m_TypeInfo.size(),
			&module.TypeInfo, &module.header.SizeOfTypeInfo);
		if (RESULT_FAIL(rs))
			AMLError::Throw(rs);
		m_MallocedMem.push_back(module.TypeInfo);
	}
	else
	{
		module.TypeInfo = NULL;
		module.header.SizeOfTypeInfo = 0;
	}

	// 모듈 테이블 생성
	rs = CreateAhaModuleTable(&module, &ModuleTable, &szTable);
	if (RESULT_FAIL(rs))
		AMLError::Throw(rs);
	m_MallocedMem.push_back(ModuleTable);

	// 파일에 쓰기
	FILE *fp = fopen(file, "wb");
	if (fp == NULL)
		throw ParseError(L"opening output file error");

	if (fwrite(ModuleTable, szTable, 1, fp) != 1)
	{
		fclose(fp);
		throw ParseError(L"writing to file error");
	}

	fclose(fp);
}
