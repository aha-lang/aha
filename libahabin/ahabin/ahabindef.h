#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <vector>
#include <string>

namespace aha
{
	typedef std::uint8_t aha_bool;
	typedef std::int8_t aha_i8;
	typedef std::uint8_t aha_u8;
	typedef std::int16_t aha_i16;
	typedef std::uint16_t aha_u16;
	typedef std::int32_t aha_i32;
	typedef std::uint32_t aha_u32;
	typedef std::int64_t aha_i64;
	typedef std::uint64_t aha_u64;
	typedef std::intptr_t aha_iptr;
	typedef float aha_f32;
	typedef double aha_f64;
	typedef std::uintptr_t aha_uptr;

	constexpr aha_bool aha_true = 1;
	constexpr aha_bool aha_false = 0;
}
