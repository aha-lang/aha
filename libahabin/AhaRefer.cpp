#include "stdafx.h"
#include "ahabin/AhaRefer.h"

namespace ahabin
{
	Result AhaRefer::Create(aha_i32 SizeOfRefer, ReadStream& strm, AhaRefer& obj)
	{
		return AhaStrings::Create(SizeOfRefer, strm, obj.m_impl);
	}

	const StringUTF16* AhaRefer::GetRefer(aha_i32 idx) const
	{
		return m_impl.GetString(idx);
	}
}
