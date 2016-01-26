#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "StringUTF16.h"

namespace ahabin
{
	class ReadStream;

	class AhaStrings
	{
	private:
		std::vector<StringUTF16> m_strings;

	public:
		Result Read(aha_i32 SizeOfStrings, ReadStream& strm);

		const std::vector<StringUTF16>& Get() const;
	};
}
