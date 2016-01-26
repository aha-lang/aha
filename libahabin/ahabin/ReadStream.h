#pragma once

#include "ahabindef.h"
#include "noncopyable.h"
#include "Result.h"

namespace aha
{
	class ReadStream : private noncopyable
	{
	public:
		virtual ~ReadStream() = default;

		virtual Result Read(void *buf, size_t size) = 0;
		virtual bool EndOfFile() = 0;
	};
}
