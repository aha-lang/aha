#include "stdafx.h"
#include "ahabin/AhaRefer.h"
#include "ahabin/ReadStream.h"

namespace ahabin
{
	Result AhaRefer::Read(aha_i32 SizeOfRefer, ReadStream& strm)
	{
		Result rs;

		ArrayList<aha_i32> refers;

		if (SizeOfRefer % sizeof(aha_i32) != 0)
			return R_BAD_IMAGE_STRINGS;

		if (RESULT_FAIL(rs = refers.Relength(SizeOfRefer / sizeof(aha_i32))))
			return rs;

		if (RESULT_FAIL(rs = strm.Read(&refers[0], SizeOfRefer)))
			return (rs == R_END_OF_FILE) ? R_BAD_IMAGE_STRINGS : rs;

		/*
		for (ahabin::aha_i32 i = 0; i < refers.Get().GetLength(); ++i)
		{
			// TODO: validate
		}
		*/

		m_refers = std::move(refers);
		return R_SUCCESS;
	}

	const ArrayList<aha_i32>& AhaRefer::Get() const
	{
		return m_refers;
	}
}
