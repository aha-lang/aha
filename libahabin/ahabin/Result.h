#pragma once

#include "ahabindef.h"

namespace ahabin
{
	enum Result : aha_i32
	{
		R_SUCCESS = 0,

		R_PROCESSED_PARTIAL = 1,
		R_FUNCTION_RETURN = 2,

		R_FAIL = -1,

		R_OUT_OF_MEMORY = -2,
		R_STACK_OVERFLOW = -3,
		R_STACK_UNDERFLOW = -4,
		R_INVALID_OPERATION = -4,
		R_INVALID_CAST = -5,

		R_BAD_HEADER = -6,
		R_BAD_STRINGS = -7,
		R_BAD_REFER = -8,
		R_BAD_NATIVE_REFER = -9,

		R_END_OF_FILE = -10,
	};

	inline bool RESULT_SUCS(Result r)
	{
		return r >= 0;
	}
	inline bool RESULT_FAIL(Result r)
	{
		return r < 0;
	}
}
