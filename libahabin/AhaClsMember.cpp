#include "stdafx.h"
#include "ahabin/AhaClsMember.h"
#include "ahabin/ReadStream.h"

namespace ahabin
{
	Result AhaClsMember::Read(ReadStream& strm, size_t& read)
	{
		Result rs;

		m_params.Relength(0);
		m_opcode.Relength(0);

		if (RESULT_FAIL(rs = strm.Read(&m_raw, sizeof(m_raw))))
			return (rs == R_END_OF_FILE) ? R_BAD_IMAGE_CLSMEM : rs;
		read += sizeof(m_raw);

		if (m_raw.type == AHA_CLSMEM_TYPE_FUNC)
		{
			if (RESULT_FAIL(rs = m_params.Relength(m_raw.function.CountOfParams)))
				return rs;
			if (RESULT_FAIL(rs = strm.Read(m_params.data(), sizeof(AhaType) * m_raw.function.CountOfParams)))
				return (rs == R_END_OF_FILE) ? R_BAD_IMAGE_CLSMEM : rs;
			read += sizeof(AhaType) * m_raw.function.CountOfParams;

			if (RESULT_FAIL(rs = m_params.Relength(m_raw.function.SizeOfOpcode)))
				return rs;
			if (RESULT_FAIL(rs = strm.Read(m_params.data(), m_raw.function.SizeOfOpcode)))
				return (rs == R_END_OF_FILE) ? R_BAD_IMAGE_CLSMEM : rs;
			read += m_raw.function.SizeOfOpcode;
		}

		return R_SUCCESS;
	}

	const AhaClsMember_raw & AhaClsMember::GetRaw() const
	{
		return m_raw;
	}

	const ArrayList<AhaType>& AhaClsMember::GetParams() const
	{
		return m_params;
	}

	const ArrayList<aha_byte>& AhaClsMember::GetOpcode() const
	{
		return m_opcode;
	}
}
