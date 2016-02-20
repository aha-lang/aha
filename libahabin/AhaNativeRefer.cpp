#include "ahabin/AhaNativeRefer.h"
#include "ahabin/exceptions.h"

namespace aha
{
	void AhaNativeRefer::Read(aha_i32 SizeOfNativeRefer, std::istream& strm)
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

	const std::vector<aha_i32>& AhaNativeRefer::Get() const
	{
		return m_impl.Get();
	}
}
