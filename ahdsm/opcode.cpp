#include "stdafx.h"

#include <ahabin/AhaOpcode.h>
#include <ahabin/AhaStrings.h>

std::wstring DisasOpcode(const std::vector<ahabin::aha_u8>& opcodes, const ahabin::AhaStrings& strings, const wchar_t* prefix)
{
	// TODO

	std::wstringstream strm;

	strm << prefix;
	for (size_t i = 0; i < opcodes.size(); ++i)
	{
		strm << std::hex << std::setfill(L'0') << std::setw(2) << (unsigned)opcodes[i];
		strm << L' ';
	}
	strm << L'\n';

	return strm.str();
}
