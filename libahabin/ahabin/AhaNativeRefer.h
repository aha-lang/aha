#pragma once

#include "ahabindef.h"
#include "AhaRefer.h"

namespace aha
{
	class AhaStrings;

	class AhaNativeRefer
	{
	private:
		AhaRefer m_impl;

	public:
		void Read(aha_u32 SizeOfNativeRefer, std::istream& strm);
		aha_u32 Write(std::ostream& strm);
		void Validate(const AhaStrings& strings) const;

		std::vector<aha_u32>& Get();
		const std::vector<aha_u32>& Get() const;
	};
}
