#include "stdafx.h"

#include <ahabin/AhaOpcode.h>
#include <ahabin/AhaStrings.h>
#include <ahabin/ArrayList.h>

std::wstring DisasOpcode(const ahabin::ArrayList<ahabin::aha_u8>& opcodes, const ahabin::AhaStrings& strings, const wchar_t* prefix)
{
	// TODO

	std::wstringstream strm;

	strm << prefix;
	for (ahabin::aha_i32 i = 0; (size_t)i < opcodes.GetLength(); ++i)
	{
		strm << std::hex << std::setfill(L'0') << std::setw(2) << (unsigned)opcodes[i];
		strm << L' ';
	}
	strm << L'\n';

	return strm.str();
}
