#include "stdafx.h"
#include "ahabin/AhaClass.h"

namespace aha
{
	void AhaClass::Read(std::istream& strm, size_t& read)
	{
		m_members.resize(0);

		strm.read((char*)&m_raw, sizeof(m_raw));
		read += sizeof(m_raw);

		m_members.resize(m_raw.CountOfMembers);

		for (aha_i32 i = 0; i < m_raw.CountOfMembers; ++i)
		{
			m_members[i].Read(strm, read);
		}
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
