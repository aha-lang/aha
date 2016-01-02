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
		static Result Create(aha_i32 SizeOfNativeRefer, ReadStream& strm, AhaNativeRefer& obj);

		const StringUTF16* GetNativeRefer(aha_i32 idx) const;
	};
}