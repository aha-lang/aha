#include "stdafx.h"
#include "Parser.h"

namespace
{
	template <typename T>
	bool range(T t1, T t2, T t3)
	{
		return (t1 <= t2) && (t2 <= t3);
	}
}

aha::AhaVariable Parser::ParseType(aha::AhaType type, const std::wstring &str)
{
	wchar_t *endptr;

	aha::AhaVariable var;

	union
	{
		long l;
		unsigned long ul;
		long long ll;
		unsigned long long ull;
		float f;
		double d;
	};

	switch (type)
	{
		case aha::AHA_TYPE_BOOL:
			if (str == L"true")
				var.v_bool = (uint32_t)true;
			else if (str == L"false")
				var.v_bool = (uint32_t)false;
			else
				throw ParseError(L"there must be a bool literal");
			break;

		case aha::AHA_TYPE_INT8:
			errno = 0;
			l = wcstol(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || !range<long>(INT8_MIN, l, INT8_MAX))
			{
				throw ParseError(L"there must be a int8 literal");
			}
			var.v_int8 = (aha::aha_i8)l;
			break;

		case aha::AHA_TYPE_UINT8:
			errno = 0;
			ul = wcstoul(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || !(ul <= UINT8_MAX))
			{
				throw ParseError(L"there must be a byte literal");
			}
			var.v_uint8 = (aha::aha_u8)ul;
			break;

		case aha::AHA_TYPE_INT16:
			errno = 0;
			l = wcstol(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || !range<long>(INT16_MIN, l, INT16_MAX))
			{
				throw ParseError(L"there must be a int16 literal");
			}
			var.v_int16 = (aha::aha_i16)l;
			break;

		case aha::AHA_TYPE_UINT16:
			errno = 0;
			ul = wcstoul(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || !(ul <= UINT16_MAX))
			{
				throw ParseError(L"there must be a uint16 literal");
			}
			var.v_uint16 = (aha::aha_u16)ul;
			break;

		case aha::AHA_TYPE_INT32:
			errno = 0;
			l = wcstol(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || !range<long>(INT32_MIN, l, INT32_MAX))
			{
				throw ParseError(L"there must be a int32 literal");
			}
			var.v_int32 = (aha::aha_i32)ul;
			break;

		case aha::AHA_TYPE_UINT32:
			errno = 0;
			ul = wcstoul(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || !(ul <= UINT32_MAX))
			{
				throw ParseError(L"there must be a uint32 literal");
			}
			var.v_uint32 = (aha::aha_u32)ul;
			break;

		case aha::AHA_TYPE_INT64:
			errno = 0;
			ll = wcstoll(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || !range<long long>(INT64_MIN, ll, INT64_MAX))
			{
				throw ParseError(L"there must be a int64 literal");
			}
			var.v_int64 = (aha::aha_i64)ll;
			break;

		case aha::AHA_TYPE_UINT64:
			errno = 0;
			ull = wcstoull(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0' || !(ul <= UINT64_MAX))
			{
				throw ParseError(L"there must be a uint64 literal");
			}
			var.v_uint64 = (aha::aha_u64)ul;
			break;

		case aha::AHA_TYPE_FLOAT32:
			errno = 0;
			f = wcstof(str.c_str(), &endptr);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a float32 literal");
			}
			var.v_float32 = (aha::aha_f32)f;
			break;

		case aha::AHA_TYPE_FLOAT64:
			errno = 0;
			d = wcstod(str.c_str(), &endptr);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a float64 literal");
			}
			var.v_float64 = (aha::aha_f64)d;
			break;

		default:
			if (str != L"null")
				throw ParseError(L"init value of object must be 'null'");
			var.v_object = nullptr;
			break;
	}

	return var;
}
