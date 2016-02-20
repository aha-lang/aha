#pragma once

#include "ahabindef.h"
#include "AhaClass.h"

namespace aha
{
	class AhaStrings;

	class AhaBody
	{
	private:
		std::vector<AhaClass> m_ClassList;

	public:
		void Read(aha_u32 SizeOfBody, std::istream& strm);
		void Validate(const AhaStrings& strings) const;

		const std::vector<AhaClass>& Get() const;
	};
}
