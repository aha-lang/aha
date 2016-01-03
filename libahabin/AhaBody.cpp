#include "stdafx.h"
#include "ahabin/AhaBody.h"
#include "ahabin/ReadStream.h"

namespace ahabin
{
	Result AhaBody::Read(aha_i32 SizeOfBody, ReadStream& strm)
	{
		Result rs;
		size_t read = 0;

		m_ClassList.Relength(0);
		while (read < SizeOfBody)
		{
			AhaClass cls;
			if (RESULT_FAIL(rs = cls.Read(strm, read)))
				return rs;

			if (read > SizeOfBody)
				return R_BAD_IMAGE_BODY;

			if (RESULT_FAIL(rs = m_ClassList.PushBack(std::move(cls))))
				return rs;
		}

		return R_SUCCESS;
	}
}
