#include "ahabin/AhaClass.h"
#include "ahabin/AhaStrings.h"
#include "ahabin/exceptions.h"

namespace aha
{
	void AhaClass::Read(std::istream& strm, size_t& read)
	{
		m_members.resize(0);

		strm.read((char*)&m_raw, sizeof(m_raw));
		read += sizeof(m_raw);

		m_members.resize(m_raw.CountOfMembers);

		for (aha_u32 i = 0; i < m_raw.CountOfMembers; ++i)
		{
			m_members[i].Read(strm, read);
		}
	}

	void AhaClass::Validate(const AhaStrings& strings)
	{
		if (!ValidateAhaAccess(m_raw.access))
			throw BadModuleClassError();
		if (!ValidateAhaClassType(m_raw.type))
			throw BadModuleClassError();
		if (!strings.ValidateString(m_raw.name))
			throw BadModuleClassError();
		if (!strings.ValidateString(m_raw.base))
			throw BadModuleClassError();
		if (!strings.ValidateString(m_raw.interfaces))
			throw BadModuleClassError();

		for (const AhaClsMember& mem : m_members)
			mem.Validate(strings);
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
