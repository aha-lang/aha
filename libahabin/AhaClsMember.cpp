#include "stdafx.h"
#include "ahabin/AhaClsMember.h"
#include "ahabin/ReadStream.h"

namespace aha
{
	Result AhaClsMember::Read(ReadStream& strm, size_t& read)
	{
		Result rs;

		aha_u8 padding[4];

		m_params.resize(0);
		m_opcode.resize(0);

		if (RESULT_FAIL(rs = strm.Read(&m_raw, sizeof(m_raw))))
			return R_BAD_IMAGE_CLSMEM;
		read += sizeof(m_raw);

		if (m_raw.type == AHA_CLSMEM_TYPE_FUNC)
		{
			m_params.resize(m_raw.function.CountOfParams);

			if (RESULT_FAIL(rs = strm.Read(m_params.data(), sizeof(AhaType) * m_raw.function.CountOfParams)))
				return R_BAD_IMAGE_CLSMEM;
			read += sizeof(AhaType) * m_raw.function.CountOfParams;

			m_opcode.resize(m_raw.function.SizeOfOpcode);

			if (RESULT_FAIL(rs = strm.Read(m_opcode.data(), m_raw.function.SizeOfOpcode)))
				return R_BAD_IMAGE_CLSMEM;
			read += m_raw.function.SizeOfOpcode;

			if (m_raw.function.SizeOfOpcode % 4 != 0)
			{
				size_t padding_sz = 4 - m_raw.function.SizeOfOpcode % 4;
				if (RESULT_FAIL(rs = strm.Read(padding, padding_sz)))
					return R_BAD_IMAGE_CLSMEM;
				read += padding_sz;
			}
		}

		return R_SUCCESS;
	}

	const AhaClsMember_raw & AhaClsMember::GetRaw() const
	{
		return m_raw;
	}

	const std::vector<AhaType>& AhaClsMember::GetParams() const
	{
		return m_params;
	}

	const std::vector<aha_u8>& AhaClsMember::GetOpcode() const
	{
		return m_opcode;
	}
}
