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
		void Validate(const AhaStrings& strings) const;

		const std::vector<aha_u32>& Get() const;
	};
}
