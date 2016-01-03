#include "stdafx.h"
#include "ahabin/AhaRefer.h"

namespace ahabin
{
	Result AhaRefer::Read(aha_i32 SizeOfRefer, ReadStream& strm, AhaRefer& obj)
	{
		Result rs = m_implRead(SizeOfRefer, strm);
		return (rs == R_BAD_IMAGE_STRINGS) ? R_BAD_IMAGE_REFER : rs;
	}

	const ArrayList<StringUTF16>& AhaRefer::Get() const
	{
		return m_impl.Get();
	}
}
