#pragma once

#include "aharundef.h"
#include <ahabin/noncopyable.h>
#include <ahabin/Result.h>
#include <ahabin/AhaType.h>

namespace aha
{
	class Stack : private noncopyable
	{
	public:
		typedef aha_u32 unit_t;

		static size_t unitof(size_t sz)
		{
			return (sz + sizeof(unit_t) - 1) & ~(sizeof(unit_t) - 1);
		}

	private:
		/*
		* 거꾸로 자라는 스택
		* ------------    <-- base
		* | previous |
		* |----------|    <-- bp
		* |    now   |
		* |----------|    <-- sp
		* |          |
		* |          |
		* ------------    <-- end
		*/
		aha_u8 *m_base, *m_end;
		aha_u8 *m_bp, *m_sp;

		aha_u32 m_MaxSize;

	public:
		explicit Stack(size_t MaxSize);
		~Stack();

		Result RegisterStackHandler();
		Result UnregisterStackHandler();

		Result Push(AhaType type, const void *pData);
		Result Pop(AhaType *pType, void *pData);
	};
}
