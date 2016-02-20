#include "ahabin/AhaRefer.h"
#include "ahabin/AhaStrings.h"
#include "ahabin/exceptions.h"

namespace aha
{
	void AhaRefer::Read(aha_u32 SizeOfRefer, std::istream& strm)
	{
		std::vector<aha_u32> refers;

		if (SizeOfRefer % sizeof(aha_u32) != 0)
			throw BadModuleReferError();

		refers.resize(SizeOfRefer / sizeof(aha_u32));
		strm.read((char*)refers.data(), SizeOfRefer);

		m_refers = std::move(refers);
	}

	void AhaRefer::Validate(const AhaStrings& strings) const
	{
		for (aha_u32 idx : m_refers)
		{
			if (!strings.ValidateString(idx))
				throw BadModuleReferError();
		}
	}

	std::vector<aha_u32>& AhaRefer::Get()
	{
		return m_refers;
	}
	const std::vector<aha_u32>& AhaRefer::Get() const
	{
		return m_refers;
	}
}
