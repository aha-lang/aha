#pragma once

#include "ahabindef.h"
#include "Result.h"

namespace aha
{
	class ReadStream;

	class AhaStrings
	{
	private:
		std::vector<std::u16string> m_strings;

	public:
		Result Read(aha_i32 SizeOfStrings, ReadStream& strm);

		const std::vector<std::u16string>& Get() const;
	};
}
