#include "stdafx.h"
#include "ahabin/AhaRefer.h"
#include "ahabin/ReadStream.h"

namespace aha
{
	Result AhaRefer::Read(aha_i32 SizeOfRefer, ReadStream& strm)
	{
		Result rs;

		std::vector<aha_i32> refers;

		if (SizeOfRefer % sizeof(aha_i32) != 0)
			return R_BAD_IMAGE_REFER;

		refers.resize(SizeOfRefer / sizeof(aha_i32));

		if (RESULT_FAIL(rs = strm.Read(refers.data(), SizeOfRefer)))
			return R_BAD_IMAGE_REFER;

		/*
		for (ahabin::aha_i32 i = 0; i < refers.Get().GetLength(); ++i)
		{
			// TODO: validate
		}
		*/

		m_refers = std::move(refers);
		return R_SUCCESS;
	}

	const std::vector<aha_i32>& AhaRefer::Get() const
	{
		return m_refers;
	}
}
