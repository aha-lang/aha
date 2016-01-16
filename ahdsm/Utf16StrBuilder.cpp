#include "stdafx.h"
#include "Utf16StrBuilder.h"

void Utf16StrBuilder::write(const ahabin::StringUTF16& str)
{
	m_str.insert(m_str.end(), &str[0], &str[str.GetLength()]);
}

void Utf16StrBuilder::write(const char16_t* str)
{
	size_t len;
	for (len = 0; str[len] != u'\x0'; ++len) { }
	m_str.insert(m_str.end(), (const ahabin::aha_u16 *)str, (const ahabin::aha_u16 *)(str + len));
}

void Utf16StrBuilder::write(const ahabin::aha_u16* begin, const ahabin::aha_u16* end)
{
	m_str.insert(m_str.end(), begin, end);
}

void Utf16StrBuilder::write_u32(ahabin::aha_u32 u32)
{
	if (u32 < 0x00010000)
	{
		m_str.push_back((ahabin::aha_u16)u32);
	}
	else
	{
		ahabin::aha_u16 x = (ahabin::aha_u16)(u32 & 0x3ff);
		ahabin::aha_u16 y = (ahabin::aha_u16)((u32 & 0xfc00) >> 10);
		ahabin::aha_u16 z = (ahabin::aha_u16)(((u32 & 0x1f0000) >> 16) - 1);

		m_str.push_back(0xd800 | (z << 6) | y);
		m_str.push_back(0xdc00 | x);
	}
}

void Utf16StrBuilder::write_u16(ahabin::aha_u16 u16)
{
	m_str.push_back(u16);
}

std::wstring Utf16StrBuilder::make()
{
	std::wstring_convert<std::codecvt_utf16<wchar_t> > conv;
	return conv.from_bytes((const char *)&m_str[0], (const char *)&m_str[m_str.size()]);
}
