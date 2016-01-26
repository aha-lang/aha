#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "AhaRefer.h"

namespace aha
{
	class AhaNativeRefer
	{
	private:
		AhaRefer m_impl;

	public:
		Result Read(aha_i32 SizeOfNativeRefer, ReadStream& strm);

		const std::vector<aha_i32>& Get() const;
	};
}
