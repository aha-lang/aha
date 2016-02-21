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
		aha_u32 Write(std::ostream& strm);

		std::vector<std::u16string>& Get();
		const std::vector<std::u16string>& Get() const;

		bool ValidateString(aha_u32 idx) const;
	};
}
