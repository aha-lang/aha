#pragma once

#include "ahabindef.h"
#include "noncopyable.h"
#include "Result.h"
#include "AhaClass.h"
#include "ArrayList.h"

namespace ahabin
{
	class ReadStream;

	class AhaBody : private noncopyable
	{
	private:
		ArrayList<AhaClass> m_ClassList;

	public:
		Result Read(aha_i32 SizeOfBody, ReadStream& strm);

		const ArrayList<AhaClass>& Get() const;
	};
}
