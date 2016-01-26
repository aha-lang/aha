#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "AhaClass.h"

namespace ahabin
{
	class ReadStream;

	class AhaBody
	{
	private:
		std::vector<AhaClass> m_ClassList;

	public:
		Result Read(aha_i32 SizeOfBody, ReadStream& strm);

		const std::vector<AhaClass>& Get() const;
	};
}
