#include "ahabin/AhaBody.h"
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

	void AhaBody::Validate(const AhaStrings& strings)
	{
		for (const AhaClass& cls : m_ClassList)
		{
			cls.Validate(strings);
		}
	}

	const std::vector<AhaClass>& AhaBody::Get() const
	{
		return m_ClassList;
	}
}
