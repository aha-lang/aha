#pragma once

#include "ahabindef.h"
#include "AhaClass.h"

namespace aha
{
	class AhaBody
	{
	private:
		std::vector<AhaClass> m_ClassList;

	public:
		void Read(aha_i32 SizeOfBody, std::istream& strm);

		const std::vector<AhaClass>& Get() const;
	};
}
