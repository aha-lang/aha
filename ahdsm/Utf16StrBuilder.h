#pragma once

#include <ahabin/StringUTF16.h>

class Utf16StrBuilder
{
private:
	std::vector<ahabin::aha_u16> m_str;

public:
	void write(const ahabin::StringUTF16& str);
	void write(const char16_t* str);
	void write(const ahabin::aha_u16* begin, const ahabin::aha_u16* end);
	void write_u32(ahabin::aha_u32 u32);
	void write_u16(ahabin::aha_u16 u16);

	std::wstring make();
};
