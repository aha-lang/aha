#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "StringUTF16.h"
#include "ArrayList.h"

namespace ahabin
{
	class ReadStream;

	class AhaStrings
	{
	private:
		ArrayList<StringUTF16> m_strings;

	public:
		Result Read(aha_i32 SizeOfStrings, ReadStream& strm);

		const ArrayList<StringUTF16>& Get() const;
	};
}
