#include <set>

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

	aha_u32 AhaClass::Write(std::ostream& strm)
	{
		aha_u32 written = 0;

		m_raw.CountOfMembers = m_members.size();

		strm.write((const char*)&m_raw, sizeof(m_raw));
		written += sizeof(m_raw);

		for (AhaClsMember& clsmem : m_members)
		{
			written += clsmem.Write(strm);
		}

		return written;
	}

	void AhaClass::Validate(const AhaStrings& strings) const
	{
		if (!ValidateAhaAccess(m_raw.access))
			throw BadModuleClassError();
		if (!ValidateAhaClassType(m_raw.type))
			throw BadModuleClassError();
		if (!strings.ValidateString(m_raw.name))
			throw BadModuleClassError();
		if (m_raw.base != 0xffffffff && !strings.ValidateString(m_raw.base))
			throw BadModuleClassError();
		if (m_raw.interfaces != 0xffffffff && !strings.ValidateString(m_raw.interfaces))
			throw BadModuleClassError();

		std::set<std::u16string> chkset;

		for (const AhaClsMember& mem : m_members)
		{
			mem.Validate(strings);

			auto rs = chkset.insert(strings.Get()[mem.GetRaw().name]);
			if (!rs.second)
				throw BadModuleClassError();
		}
	}

	AhaClass_raw &AhaClass::GetRaw()
	{
		return m_raw;
	}
	const AhaClass_raw &AhaClass::GetRaw() const
	{
		return m_raw;
	}

	std::vector<AhaClsMember>& AhaClass::GetMembers()
	{
		return m_members;
	}
	const std::vector<AhaClsMember>& AhaClass::GetMembers() const
	{
		return m_members;
	}
}
