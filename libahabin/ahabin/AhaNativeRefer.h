#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "AhaRefer.h"

namespace ahabin
{
	class AhaNativeRefer
	{
	private:
		AhaRefer m_impl;

	public:
		Result Read(aha_i32 SizeOfNativeRefer, ReadStream& strm);

		const ArrayList<aha_i32>& Get() const;
	};
}
