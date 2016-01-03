#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "AhaStrings.h"

namespace ahabin
{
	class AhaNativeRefer
	{
	private:
		AhaStrings m_impl;

	public:
		Result Read(aha_i32 SizeOfNativeRefer, ReadStream& strm);

		const ArrayList<StringUTF16>& Get() const;
	};
}
