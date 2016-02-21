#include <set>

#include "ahabin/AhaBody.h"
#include "ahabin/AhaStrings.h"
#include "ahabin/exceptions.h"

namespace aha
{
	void AhaBody::Read(aha_u32 SizeOfBody, std::istream& strm)
	{
		size_t read = 0, szofbody = (size_t)SizeOfBody;

		m_ClassList.resize(0);
		while (read < szofbody)
		{
			AhaClass cls;
			cls.Read(strm, read);

			if (read > szofbody)
				throw BadModuleBodyError();

			m_ClassList.push_back(std::move(cls));
		}
	}

	aha_u32 AhaBody::Write(std::ostream& strm)
	{
		aha_u32 written = 0;

		for (AhaClass& cls : m_ClassList)
		{
			written += cls.Write(strm);
		}

		return written;
	}

	void AhaBody::Validate(const AhaStrings& strings) const
	{
		std::set<std::u16string> chkset;

		for (const AhaClass& cls : m_ClassList)
		{
			cls.Validate(strings);

			auto rs = chkset.insert(strings.Get()[cls.GetRaw().name]);
			if (!rs.second)
				throw BadModuleBodyError();
		}
	}

	std::vector<AhaClass>& AhaBody::Get()
	{
		return m_ClassList;
	}
	const std::vector<AhaClass>& AhaBody::Get() const
	{
		return m_ClassList;
	}
}
