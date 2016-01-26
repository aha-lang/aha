#include "stdafx.h"
#include "ahabin/AhaClass.h"
#include "ahabin/ReadStream.h"

namespace ahabin
{
	Result AhaClass::Read(ReadStream& strm, size_t& read)
	{
		Result rs;

		m_members.resize(0);

		if (RESULT_FAIL(rs = strm.Read(&m_raw, sizeof(m_raw))))
			return R_BAD_IMAGE_CLASS;
		read += sizeof(m_raw);

		m_members.resize(m_raw.CountOfMembers);

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

	const std::vector<AhaClsMember>& AhaClass::GetMembers() const
	{
		return m_members;
	}
}
