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

		R_IO_ERROR = -2,

		R_OUT_OF_MEMORY = -3,
		R_STACK_OVERFLOW = -4,
		R_STACK_UNDERFLOW = -5,
		R_INVALID_OPERATION = -6,
		R_INVALID_CAST = -7,

		R_BAD_IMAGE_HEADER = -8,
		R_BAD_IMAGE_STRINGS = -9,
		R_BAD_IMAGE_REFER = -10,
		R_BAD_IMAGE_NATIVE_REFER = -11,
		R_BAD_IMAGE_BODY = -12,
		R_BAD_IMAGE_CLASS = -13,
		R_BAD_IMAGE_CLSMEM = -14,

		R_END_OF_FILE = -15,
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
