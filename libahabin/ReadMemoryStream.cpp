#include "stdafx.h"
#include "ahabin/ReadMemoryStream.h"

namespace ahabin
{
	Result ReadMemoryStream::Create(const void *pMem, size_t size)
	{
		m_pMemory = (const char *)pMem;
		m_pNow = (const char *)pMem;
		m_size = size;
		m_left = size;
		return R_SUCCESS;
	}

	Result ReadMemoryStream::Read(void *buf, size_t size)
	{
		size_t left = m_left;

		if (m_left < size)
			return R_END_OF_FILE;

		m_left -= size;
		memcpy(buf, m_pNow, size);
		m_pNow += size;

		return R_SUCCESS;
	}

	bool ReadMemoryStream::EndOfFile()
	{
		return (m_left == 0);
	}
}
