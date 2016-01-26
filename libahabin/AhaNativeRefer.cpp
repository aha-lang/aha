#include "stdafx.h"
#include "ahabin/AhaNativeRefer.h"

namespace ahabin
{
	Result AhaNativeRefer::Read(aha_i32 SizeOfNativeRefer, ReadStream& strm)
	{
		Result rs = m_impl.Read(SizeOfNativeRefer, strm);
		return (rs == R_BAD_IMAGE_REFER) ? R_BAD_IMAGE_NATIVE_REFER : rs;
	}

	const std::vector<aha_i32>& AhaNativeRefer::Get() const
	{
		return m_impl.Get();
	}
}
