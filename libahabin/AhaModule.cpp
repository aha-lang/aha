#include "stdafx.h"
#include "ahabin/AhaModule.h"
#include "ahabin/ReadStream.h"

namespace
{
	static constexpr ahabin::aha_u8 abf_mark[4] = { '\0', 'A', 'H', 'E' };
}

namespace ahabin
{
	Result AhaModule::Read(ReadStream& strm)
	{
		Result rs;

		if (RESULT_FAIL(rs = strm.Read(&m_Header, sizeof(m_Header))))
			return (rs == R_END_OF_FILE) ? R_BAD_IMAGE_HEADER : rs;

		if (memcmp(&m_Header.mark, &abf_mark, sizeof(abf_mark)) != 0)
			return R_BAD_IMAGE_HEADER;

		if (RESULT_FAIL(rs = m_Strings.Read(m_Header.SizeOfStrings, strm)))
			return rs;

		if (RESULT_FAIL(rs = m_Refer.Read(m_Header.SizeOfRefer, strm)))
			return rs;

		if (RESULT_FAIL(rs = m_NativeRefer.Read(m_Header.SizeOfNativeRefer, strm)))
			return rs;

		if (RESULT_FAIL(rs = m_Body.Read(m_Header.SizeOfBody, strm)))
			return rs;

		return R_SUCCESS;
	}

	AhaStrings& AhaModule::GetStrings()
	{
		return m_Strings;
	}

	AhaRefer& AhaModule::GetRefer()
	{
		return m_Refer;
	}

	AhaNativeRefer& AhaModule::GetNativeRefer()
	{
		return m_NativeRefer;
	}

	AhaBody& AhaModule::GetBody()
	{
		return m_Body;
	}
}
