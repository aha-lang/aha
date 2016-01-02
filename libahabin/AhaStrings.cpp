#include "stdafx.h"
#include "ahabin/AhaStrings.h"
#include "ahabin/ReadStream.h"
#include "util.h"

namespace ahabin
{
	Result AhaStrings::Create(aha_i32 SizeOfStrings, ReadStream& strm, AhaStrings& obj)
	{
		Result rs;

		ArrayList<StringUTF16> strings;

		size_t read = 0;

		while (read < SizeOfStrings)
		{
			StringUTF16 str;
			aha_i32 size;

			read += sizeof(size);
			if (read >= SizeOfStrings)
				return R_BAD_STRINGS;
			if (RESULT_FAIL(rs = strm.Read(&size, sizeof(size))))
				return rs;

			if (size % sizeof(aha_i16) != 0)
				return R_BAD_STRINGS;

			read += size;
			if (read > SizeOfStrings)
				return R_BAD_STRINGS;
			if (RESULT_FAIL(rs = StringUTF16::Create(strm, size / sizeof(aha_i16), str)))
				return R_BAD_STRINGS;

			strings.PushBack(std::move(str));
		}

		if (read != SizeOfStrings)
			return R_BAD_STRINGS;

		obj.m_strings = std::move(strings);
		return R_SUCCESS;
	}

	const StringUTF16* AhaStrings::GetString(aha_i32 idx) const
	{
		if (idx < m_strings.GetLength())
			return &m_strings[idx];
		else
			return nullptr;
	}
}
