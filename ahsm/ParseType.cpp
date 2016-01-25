#include "stdafx.h"
#include "Parser.h"

void *Parser::ParseType(AhaTypeKind tkind, const std::wstring &str, size_t *psz)
{
	uint32_t *ptr;
	wchar_t *endptr;
	union
	{
		unsigned long ul;
		unsigned long long ull;
		long l;
		long long ll;
		float f;
		double d;
	} var;

	switch (tkind)
	{
		case AHA_TYPE_BOOL:
			ptr = (uint32_t *)m_MemPool.malloc();
			if (ptr == nullptr)
				throw std::bad_alloc();

			if (str == L"true")
				*ptr = (uint32_t)true;
			else if (str == L"false")
				*ptr = (uint32_t)false;
			else
				throw ParseError(L"there must be a bool literal");

			*psz = sizeof(bool);
			break;
		case AHA_TYPE_BYTE:
			ptr = (uint32_t *)m_MemPool.malloc();
			if (ptr == nullptr)
				throw std::bad_alloc();

			errno = 0;
			var.ul = wcstoul(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a byte literal");
			}
			*(uint8_t *)ptr = (uint8_t)var.ul;

			*psz = sizeof(uint8_t);
			break;
		case AHA_TYPE_INT8:
			ptr = (uint32_t *)m_MemPool.malloc();
			if (ptr == nullptr)
				throw std::bad_alloc();

			errno = 0;
			var.l = wcstol(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a int8 literal");
			}
			*(int8_t *)ptr = (int8_t)var.ul;

			*psz = sizeof(int8_t);
			break;
		case AHA_TYPE_INT16:
			ptr = (uint32_t *)m_MemPool.malloc();
			if (ptr == nullptr)
				throw std::bad_alloc();

			errno = 0;
			var.l = wcstol(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a int16 literal");
			}
			*(int16_t *)ptr = (int16_t)var.ul;

			*psz = sizeof(int16_t);
			break;
		case AHA_TYPE_INT32:
		case AHA_TYPE_INTPTR:
			ptr = (uint32_t *)m_MemPool.malloc();
			if (ptr == nullptr)
				throw std::bad_alloc();

			errno = 0;
			var.l = wcstol(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a int32 literal");
			}
			*(int32_t *)ptr = (int32_t)var.ul;

			*psz = sizeof(int32_t);
			break;
		case AHA_TYPE_INT64:
			ptr = (uint32_t *)m_MemPool.malloc();
			if (ptr == nullptr)
				throw std::bad_alloc();

			errno = 0;
			var.ll = wcstoll(str.c_str(), &endptr, 0);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a int64 literal");
			}
			*(int64_t *)ptr = (int64_t)var.ul;

			*psz = sizeof(int64_t);
			break;
		case AHA_TYPE_FLOAT32:
			ptr = (uint32_t *)m_MemPool.malloc();
			if (ptr == nullptr)
				throw std::bad_alloc();

			errno = 0;
			var.d = wcstod(str.c_str(), &endptr);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a float32 literal");
			}
			*(float *)ptr = (float)var.f;

			*psz = sizeof(float);
			break;
		case AHA_TYPE_FLOAT64:
			ptr = (uint32_t *)m_MemPool.ordered_malloc(2);
			if (ptr == nullptr)
				throw std::bad_alloc();

			errno = 0;
			var.d = wcstod(str.c_str(), &endptr);
			if (errno == ERANGE || *endptr != L'\0')
			{
				throw ParseError(L"there must be a float64 literal");
			}
			*(double *)ptr = var.d;

			*psz = sizeof(double);
			break;
		case AHA_TYPE_OBJECT:
			if (str != L"null")
				throw ParseError(L"init value of object must be 'null'");
			ptr = (uint32_t *)m_MemPool.malloc();
			if (ptr == nullptr)
				throw std::bad_alloc();

			*ptr = 0;

			*psz = 4;
			break;
	}

	return ptr;
}
