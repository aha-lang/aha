#pragma once

#include "ahabindef.h"
#include "noncopyable.h"
#include "Result.h"
#include "ArrayList.h"

namespace ahabin
{
	class ReadStream;

	class AhaRefer : private noncopyable
	{
	private:
		ArrayList<aha_i32> m_refers;

	public:
		Result Read(aha_i32 SizeOfRefer, ReadStream& strm);

		const ArrayList<aha_i32>& Get() const;
	};
}
