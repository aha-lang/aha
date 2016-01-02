#include "stdafx.h"
#include "ahabin/MemoryStream.h"

namespace ahabin
{
	MemoryStream::MemoryStream(const void *pMem, size_t size)
		: m_pMemory((const char *)pMem), m_pNow((const char *)pMem)
		, m_size(size), m_left(size)
	{
	}

	Result MemoryStream::Read(void *buf, size_t size)
	{
		size_t left = m_left;

		if (m_left < size)
			return R_END_OF_FILE;

		m_left -= size;
		memcpy(buf, m_pNow, size);
		m_pNow += size;

		return R_SUCCESS;
	}
}
