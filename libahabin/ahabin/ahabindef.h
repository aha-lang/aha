#pragma once

#include <stdlib.h>
#include <stdint.h>

#include <memory>

namespace ahabin
{
	typedef std::uint8_t aha_bool;
	typedef std::uint8_t aha_byte;
	typedef std::int8_t aha_i8;
	typedef std::int16_t aha_i16;
	typedef std::int32_t aha_i32;
	typedef std::int64_t aha_i64;
	typedef std::intptr_t aha_iptr;
	typedef float aha_f32;
	typedef double aha_f64;

	constexpr aha_bool aha_true = 1;
	constexpr aha_bool aha_false = 0;
}
