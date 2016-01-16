#include "stdafx.h"
#include "ahabin\StringUTF16.h"
#include "util.h"

namespace ahabin
{
	StringUTF16::StringUTF16()
		: m_str(nullptr), m_length(0)
	{
	}

	StringUTF16::~StringUTF16()
	{
		free(m_str);
	}

	StringUTF16::StringUTF16(StringUTF16&& other)
	{
		m_str = other.m_str;
		m_length = other.m_length;
		other.m_str = nullptr;
		other.m_length = 0;
	}

	StringUTF16& StringUTF16::operator =(StringUTF16&& other)
	{
		StringUTF16(std::move(other)).swap(*this);
		return *this;
	}

	void StringUTF16::swap(StringUTF16 & other)
	{
		std::swap(m_str, other.m_str);
		std::swap(m_length, other.m_length);
	}

	Result StringUTF16::Create(const aha_u16* str, StringUTF16& obj)
	{
		free(obj.m_str);

		aha_i32 length;
		for (length = 0; str[length] != 0; ++length) { }

		util::malloc_unique_ptr<aha_u16> newstr((aha_u16*)malloc(sizeof(aha_u16) * length));
		if (newstr == nullptr)
			return R_OUT_OF_MEMORY;

		memcpy(newstr.get(), str, sizeof(aha_u16) * length);

		obj.m_str = newstr.release();
		obj.m_length = length;
		return R_SUCCESS;
	}

	Result StringUTF16::Create(ReadStream& strm, aha_i32 length, StringUTF16& obj)
	{
		free(obj.m_str);

		Result rs;

		util::malloc_unique_ptr<aha_u16> newstr((aha_u16*)malloc(sizeof(aha_u16) * length));
		if (newstr == nullptr)
			return R_OUT_OF_MEMORY;

		if (RESULT_FAIL(rs = strm.Read(newstr.get(), length)))
			return rs;

		obj.m_str = newstr.release();
		obj.m_length = length;
		return R_SUCCESS;
	}

	aha_i32 StringUTF16::GetLength() const
	{
		return m_length;
	}

	aha_u16& StringUTF16::operator[](aha_i32 idx)
	{
		return const_cast<aha_u16&>(static_cast<const StringUTF16&>(*this)[idx]);
	}

	const aha_u16& StringUTF16::operator[](aha_i32 idx) const
	{
		return m_str[idx];
	}
}
