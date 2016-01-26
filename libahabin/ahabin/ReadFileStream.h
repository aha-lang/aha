#pragma once

#include "ahabindef.h"
#include "ReadStream.h"
#include "Result.h"

namespace aha
{
	class ReadFileStream : public ReadStream
	{
	private:
		FILE* m_fp = nullptr;

	public:
		Result Create(const char* filename);

		virtual Result Read(void *buf, size_t size) override;
		virtual bool EndOfFile() override;
	};
}