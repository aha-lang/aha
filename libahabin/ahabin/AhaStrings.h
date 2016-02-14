#pragma once

#include "ahabindef.h"

namespace aha
{
	class AhaStrings
	{
	private:
		std::vector<std::u16string> m_strings;

	public:
		void Read(aha_i32 SizeOfStrings, std::istream& strm);

		const std::vector<std::u16string>& Get() const;
	};
}
