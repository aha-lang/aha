#include "stdafx.h"
#include "ahabin/AhaBody.h"
#include "ahabin/ReadStream.h"

namespace aha
{
	Result AhaBody::Read(aha_i32 SizeOfBody, ReadStream& strm)
	{
		Result rs;
		size_t read = 0, szofbody = (size_t)SizeOfBody;

		m_ClassList.resize(0);
		while (read < szofbody)
		{
			AhaClass cls;
			if (RESULT_FAIL(rs = cls.Read(strm, read)))
				return rs;

			if (read > szofbody)
				return R_BAD_IMAGE_BODY;

			m_ClassList.push_back(std::move(cls));
		}

		return R_SUCCESS;
	}

	const std::vector<AhaClass>& AhaBody::Get() const
	{
		return m_ClassList;
	}
}
