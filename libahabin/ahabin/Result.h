#pragma once

#include "ahabindef.h"

namespace aha
{
	enum Result : aha_i32
	{
		R_SUCCESS = 0,

		R_PROCESSED_PARTIAL = 1,
		R_FUNCTION_RETURN = 2,

		R_FAIL = -1,

		R_IO_ERROR = -2,
		R_STACK_OVERFLOW = -3,
		R_STACK_UNDERFLOW = -4,
		R_INVALID_CAST = -5,
		R_END_OF_FILE = -6,

		R_BAD_IMAGE_HEADER = -7,
		R_BAD_IMAGE_STRINGS = -8,
		R_BAD_IMAGE_REFER = -9,
		R_BAD_IMAGE_NATIVE_REFER = -10,
		R_BAD_IMAGE_BODY = -11,
		R_BAD_IMAGE_CLASS = -12,
		R_BAD_IMAGE_CLSMEM = -13,
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
