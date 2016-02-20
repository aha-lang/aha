#pragma once

#include "ahabindef.h"

namespace aha
{
	class AhaStrings;

	class AhaRefer
	{
	private:
		std::vector<aha_u32> m_refers;

	public:
		void Read(aha_u32 SizeOfRefer, std::istream& strm);
		void Validate(const AhaStrings& strings) const;

		std::vector<aha_u32>& Get();
		const std::vector<aha_u32>& Get() const;
	};
}
