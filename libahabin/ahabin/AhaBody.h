#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "AhaClass.h"
#include "ArrayList.h"

namespace ahabin
{
	class ReadStream;

	class AhaBody
	{
	private:
		ArrayList<AhaClass> m_ClassList;

	public:
		static Result Create(aha_i32 SizeOfBody, ReadStream& strm, AhaBody& obj);
	};
}
