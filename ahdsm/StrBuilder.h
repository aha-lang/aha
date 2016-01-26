#pragma once

class StrBuilder
{
private:
	std::wstringstream m_strm;

public:
	void write(const char16_t* str);
	void write(const char16_t* begin, const char16_t* end);
	void write(const std::wstring& str);

	std::wstring make();
};
