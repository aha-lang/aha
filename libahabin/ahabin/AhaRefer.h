#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "AhaStrings.h"

namespace ahabin
{
	class AhaRefer
	{
	private:
		AhaStrings m_impl;

	public:
		Result Read(aha_i32 SizeOfRefer, ReadStream& strm);

		const ArrayList<StringUTF16>& Get() const;
	};
}
