#include "ahabin/AhaNativeRefer.h"
#include "ahabin/exceptions.h"

namespace aha
{
	void AhaNativeRefer::Read(aha_u32 SizeOfNativeRefer, std::istream& strm)
	{
		try
		{
			m_impl.Read(SizeOfNativeRefer, strm);
		}
		catch (BadModuleReferError&)
		{
			throw BadModuleNativeReferError();
		}
	}

	aha_u32 AhaNativeRefer::Write(std::ostream& strm)
	{
		return m_impl.Write(strm);
	}

	void AhaNativeRefer::Validate(const AhaStrings& strings) const
	{
		m_impl.Validate(strings);
	}

	std::vector<aha_u32>& AhaNativeRefer::Get()
	{
		return m_impl.Get();
	}
	const std::vector<aha_u32>& AhaNativeRefer::Get() const
	{
		return m_impl.Get();
	}
}
