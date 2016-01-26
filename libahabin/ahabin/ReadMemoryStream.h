#pragma once

#include "ahabindef.h"
#include "ReadStream.h"
#include "Result.h"

namespace aha
{
	class ReadMemoryStream : public ReadStream
	{
	private:
		const char *m_pMemory = nullptr, *m_pNow;
		size_t m_size, m_left;

	public:
		Result Create(const void *pMem, size_t size);

		virtual Result Read(void *buf, size_t size) override;
		virtual bool EndOfFile() override;
	};
}
