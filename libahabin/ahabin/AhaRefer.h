#pragma once

#include "ahabindef.h"

namespace aha
{
	class AhaRefer
	{
	private:
		std::vector<aha_i32> m_refers;

	public:
		void Read(aha_i32 SizeOfRefer, std::istream& strm);

		const std::vector<aha_i32>& Get() const;
	};
}
