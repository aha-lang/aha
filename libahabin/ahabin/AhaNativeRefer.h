#pragma once

#include "ahabindef.h"
#include "AhaRefer.h"

namespace aha
{
	class AhaNativeRefer
	{
	private:
		AhaRefer m_impl;

	public:
		void Read(aha_i32 SizeOfNativeRefer, std::istream& strm);

		const std::vector<aha_i32>& Get() const;
	};
}
