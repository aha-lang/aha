#include "stdafx.h"
#include "ahabin/AhaNativeRefer.h"

namespace ahabin
{
	Result AhaNativeRefer::Create(aha_i32 SizeOfNativeRefer, ReadStream& strm, AhaNativeRefer& obj)
	{
		return AhaStrings::Create(SizeOfNativeRefer, strm, obj.m_impl);
	}

	const StringUTF16* AhaNativeRefer::GetNativeRefer(aha_i32 idx) const
	{
		return m_impl.GetString(idx);
	}
}
