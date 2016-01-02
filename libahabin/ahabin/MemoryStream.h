#pragma once

#include "ahabindef.h"
#include "ReadStream.h"
#include "Result.h"

namespace ahabin
{
	class MemoryStream : public ReadStream
	{
	private:
		const char *m_pMemory, *m_pNow;
		size_t m_size, m_left;
	public:
		explicit MemoryStream(const void *pMem, size_t size);
		virtual Result Read(void *buf, size_t size) override;
	};
}
