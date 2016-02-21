#include "stdafx.h"
#include "Parser.h"

#include <ahabin/AhaModule.h>

void Parser::Save(const char *file)
{
	aha::AhaModule module;
	module.GetHeader().ModuleName = m_ModuleName;

	// strings
	for (const std::wstring& str : m_strtbl)
	{
		module.GetStrings().Get().push_back(boost::locale::conv::utf_to_utf<char16_t>(str));
	}

	module.GetRefer().Get() = m_Refer;
	module.GetNativeRefer().Get() = m_NativeRefer;
	module.GetBody().Get() = m_ClassList;

	module.Validate();

	std::ofstream f(file, std::ios::out | std::ios::binary | std::ios::trunc);
	module.Write(f);
}
