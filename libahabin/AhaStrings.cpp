#include "stdafx.h"
#include "ahabin/AhaStrings.h"
#include "ahabin/ReadStream.h"
#include "util.h"

namespace aha
{
	Result AhaStrings::Read(aha_i32 SizeOfStrings, ReadStream& strm)
	{
		Result rs;

		std::vector<std::u16string> strings;
		aha_u8 padding[4];

		size_t read = 0, szofstr = (size_t)SizeOfStrings;

		while (read < szofstr)
		{
			std::u16string str;
			aha_i32 size;

			read += sizeof(size);
			if (read >= szofstr)
				return R_BAD_IMAGE_STRINGS;
			if (RESULT_FAIL(rs = strm.Read(&size, sizeof(size))))
				return R_BAD_IMAGE_STRINGS;

			if (size % sizeof(aha_i16) != 0)
				return R_BAD_IMAGE_STRINGS;

			read += size;
			if (read > szofstr)
				return R_BAD_IMAGE_STRINGS;

			str.resize(size / sizeof(char16_t));
			if (RESULT_FAIL(rs = strm.Read(&str[0], size)))
				return R_BAD_IMAGE_STRINGS;

			if (size % 4 != 0)
			{
				size_t padding_sz = 4 - size % 4;
				if (RESULT_FAIL(rs = strm.Read(padding, padding_sz)))
					return R_BAD_IMAGE_STRINGS;
				read += padding_sz;
			}

			strings.push_back(std::move(str));
		}

		if (read != szofstr)
			return R_BAD_IMAGE_STRINGS;

		m_strings = std::move(strings);
		return R_SUCCESS;
	}

	const std::vector<std::u16string>& AhaStrings::Get() const
	{
		return m_strings;
	}
}
