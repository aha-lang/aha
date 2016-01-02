#pragma once

#include "ahabindef.h"
#include "Result.h"

namespace ahabin
{
	class ReadStream
	{
	public:
		virtual ~ReadStream() = 0;
		virtual Result Read(void *buf, size_t size) = 0;
	};
}
