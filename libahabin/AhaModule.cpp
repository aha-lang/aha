#include "stdafx.h"
#include "ahabin/AhaModule.h"
#include "ahabin/ReadStream.h"

namespace
{
	static constexpr ahabin::aha_byte abf_mark[4] = { '\0', 'A', 'H', 'E' };
}

namespace ahabin
{
	Result AhaModule::Create(ReadStream& strm, AhaModule& obj)
	{
		Result rs;

		if (RESULT_FAIL(rs = strm.Read(&obj.m_Header, sizeof(obj.m_Header))))
			return rs;

		if (memcmp(&obj.m_Header.mark, &abf_mark, sizeof(abf_mark)) != 0)
			return R_BAD_HEADER;

		if (RESULT_FAIL(rs = AhaStrings::Create(obj.m_Header.SizeOfStrings, strm, obj.m_Strings)))
			return rs;

		if (RESULT_FAIL(rs = AhaRefer::Create(obj.m_Header.SizeOfRefer, strm, obj.m_Refer)))
			return rs;

		if (RESULT_FAIL(rs = AhaNativeRefer::Create(obj.m_Header.SizeOfNativeRefer, strm, obj.m_NativeRefer)))
			return rs;

		if (RESULT_FAIL(rs = AhaBody::Create(obj.m_Header.SizeOfBody, strm, obj.m_Body)))
			return rs;

		return R_SUCCESS;
	}
}
