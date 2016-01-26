#pragma once

#include "ahabindef.h"
#include "Result.h"

namespace ahabin
{
	class ReadStream;

	class AhaRefer
	{
	private:
		std::vector<aha_i32> m_refers;

	public:
		Result Read(aha_i32 SizeOfRefer, ReadStream& strm);

		const std::vector<aha_i32>& Get() const;
	};
}
