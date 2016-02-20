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

	void AhaNativeRefer::Validate(const AhaStrings& strings)
	{
		m_impl.Validate(strings);
	}

	const std::vector<aha_u32>& AhaNativeRefer::Get() const
	{
		return m_impl.Get();
	}
}
