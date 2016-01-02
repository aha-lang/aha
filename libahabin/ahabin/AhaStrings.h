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
		static Result Create(aha_i32 SizeOfStrings, ReadStream& strm, AhaStrings& obj);

		const StringUTF16* GetString(aha_i32 idx) const;
	};
}
