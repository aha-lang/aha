#pragma once

#include "ahabindef.h"

namespace aha
{
	class AhaStrings
	{
	private:
		std::vector<std::u16string> m_strings;

	public:
		void Read(aha_u32 SizeOfStrings, std::istream& strm);

		const std::vector<std::u16string>& Get() const;

		bool ValidateString(aha_u32 idx) const;
	};
}
