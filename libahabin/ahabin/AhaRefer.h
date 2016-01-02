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
		static Result Create(aha_i32 SizeOfRefer, ReadStream& strm, AhaRefer& obj);

		const StringUTF16* GetRefer(aha_i32 idx) const;
	};
}