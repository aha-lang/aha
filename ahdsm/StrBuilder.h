#pragma once

#include <ahabin/StringUTF16.h>

class StrBuilder
{
private:
	std::wstringstream m_strm;

public:
	void write(const ahabin::StringUTF16& str);
	void write(const char16_t* str);
	void write(const ahabin::aha_u16* begin, const ahabin::aha_u16* end);
	void write(const char16_t* begin, const char16_t* end);
	void write(const std::wstring& str);

	std::wstring make();
};
