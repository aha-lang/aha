#include "stdafx.h"
#include "ahabin/AhaStrings.h"
#include "ahabin/ReadStream.h"
#include "util.h"

namespace ahabin
{
	Result AhaStrings::Read(aha_i32 SizeOfStrings, ReadStream& strm)
	{
		Result rs;

		ArrayList<StringUTF16> strings;

		size_t read = 0, szofstr = (size_t)SizeOfStrings;

		while (read < szofstr)
		{
			StringUTF16 str;
			aha_i32 size;

			read += sizeof(size);
			if (read >= szofstr)
				return R_BAD_IMAGE_STRINGS;
			if (RESULT_FAIL(rs = strm.Read(&size, sizeof(size))))
				return (rs == R_END_OF_FILE) ? R_BAD_IMAGE_STRINGS : rs;

			if (size % sizeof(aha_i16) != 0)
				return R_BAD_IMAGE_STRINGS;

			read += size;
			if (read > szofstr)
				return R_BAD_IMAGE_STRINGS;
			if (RESULT_FAIL(rs = StringUTF16::Create(strm, size / sizeof(aha_i16), str)))
				return rs;

			strings.PushBack(std::move(str));
		}

		if (read != szofstr)
			return R_BAD_IMAGE_STRINGS;

		m_strings = std::move(strings);
		return R_SUCCESS;
	}

	const ArrayList<StringUTF16>& AhaStrings::Get() const
	{
		return m_strings;
	}
}
