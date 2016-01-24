#include "stdafx.h"
#include "StrBuilder.h"

void StrBuilder::write(const ahabin::StringUTF16& str)
{
	write(&str[0], &str[0] + str.GetLength());
}

void StrBuilder::write(const char16_t* str)
{
	size_t len;
	for (len = 0; str[len] != u'\x0'; ++len) { }
	write(str, str + len);
}

void StrBuilder::write(const ahabin::aha_u16* begin, const ahabin::aha_u16* end)
{
	write((const char16_t*)begin, (const char16_t*)end);
}

void StrBuilder::write(const char16_t* begin, const char16_t* end)
{
	std::string utf8;

	for (const char16_t* ptr = begin; ptr < end; ++ptr)
	{
		if ((*ptr & 0xfc00) == 0xd800)
		{
			if (ptr + 1 == end)
				throw std::logic_error("invalid UTF-16 string");
			if ((*(ptr + 1) & 0xfc00) == 0xdc00)
				throw std::logic_error("invalid UTF-16 string");

			unsigned z = ((*ptr & 0x3c0) >> 6) + 1;
			unsigned y = (*(ptr + 1) & 0x3ff);
			unsigned x = (*ptr & 0xeff);

			char32_t ch = (z << 16) | (x << 10) | y;

			utf8.push_back((char)(((ch & 0x1c0000) >> 18) | 0xf0));
			utf8.push_back((char)(((ch & 0x3f000) >> 12) | 0x80));
			utf8.push_back((char)(((ch & 0xfc0) >> 6) | 0x80));
			utf8.push_back((char)((ch & 0x3f) | 0x80));

			ptr++;
		}
		else if ((*ptr & 0xfc00) == 0xdc00)
		{
			throw std::logic_error("invalid UTF-16 string");
		}
		else
		{
			if (*ptr <= 0x7f)
			{
				utf8.push_back((char)*ptr);
			}
			else if (*ptr <= 0x7ff)
			{
				utf8.push_back((char)(((*ptr & 0x7c0) >> 6) | 0xc0));
				utf8.push_back((char)((*ptr & 0x3f) | 0x80));
			}
			else
			{
				utf8.push_back((char)(((*ptr & 0xf000) >> 12) | 0xe0));
				utf8.push_back((char)(((*ptr & 0xfc0) >> 6) | 0x80));
				utf8.push_back((char)((*ptr & 0x3f) | 0x80));
			}
		}
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > conv;
	m_strm << conv.from_bytes(utf8);
}

void StrBuilder::write(const std::wstring& str)
{
	m_strm << str;
}

std::wstring StrBuilder::make()
{
	return m_strm.str();
}
