#include "stdafx.h"
#include "ahabin/AhaClass.h"
#include "ahabin/ReadStream.h"

namespace ahabin
{
	Result AhaClass::Read(ReadStream& strm, size_t& read)
	{
		Result rs;

		m_members.Relength(0);

		if (RESULT_FAIL(rs = strm.Read(&m_raw, sizeof(m_raw))))
			return (rs == R_END_OF_FILE) ? R_BAD_IMAGE_CLASS : rs;
		read += sizeof(m_raw);

		if (RESULT_FAIL(rs = m_members.Relength(m_raw.CountOfMembers)))
			return rs;
		for (aha_i32 i = 0; i < m_raw.CountOfMembers; ++i)
		{
			if (RESULT_FAIL(rs = m_members[i].Read(strm, read)))
				return rs;
		}

		return R_SUCCESS;
	}

	const AhaClass_raw AhaClass::GetRaw() const
	{
		return m_raw;
	}

	const ArrayList<AhaClsMember>& AhaClass::GetMembers() const
	{
		return m_members;
	}
}
