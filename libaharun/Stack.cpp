#include "stdafx.h"
#include "aharun/Stack.h"

#include <Windows.h>

namespace
{
	DWORD s_PageSize;
	PVOID s_Handler;

	bool CommitGuardPage(void *addr)
	{
		DWORD oldattr;
		if (VirtualAlloc(addr, s_PageSize, MEM_COMMIT, PAGE_READWRITE) != NULL)
		{
			if (VirtualProtect(addr, s_PageSize, PAGE_READWRITE | PAGE_GUARD, &oldattr))
			{
				return true;
			}
		}
		return false;
	}

	LONG CALLBACK StackOnSEHExcept(PEXCEPTION_POINTERS exc)
	{
		if (exc->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION)
		{
			ULONG_PTR addr;

			addr = exc->ExceptionRecord->ExceptionInformation[1];
			addr -= s_PageSize;
			addr -= addr % s_PageSize;

			if (CommitGuardPage((LPVOID)addr))
			{
				return EXCEPTION_CONTINUE_EXECUTION;
			}

			/* fail : StackOverflow */
			OutputDebugString(
				L">>abort<< To commit/Guard new page is fail. It means that stack overflows.\n"
				L"Please write code that throws StackOverflowException here.\n"
				);
			return EXCEPTION_CONTINUE_SEARCH;
		}
		else
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}
	}
}

namespace aha
{
	Stack::Stack(size_t MaxSize)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		s_PageSize = si.dwPageSize;

		m_MaxSize = (MaxSize + s_PageSize - 1) / s_PageSize * s_PageSize;

		m_end = (aha_u8*)VirtualAlloc(NULL, MaxSize, MEM_RESERVE, PAGE_READWRITE);
		if (m_end != nullptr)
		{
			m_bp = m_sp = m_base = m_end + MaxSize;

			aha_u8* page = m_base - s_PageSize;
			if (VirtualAlloc(page, s_PageSize, MEM_COMMIT, PAGE_READWRITE) != nullptr)
			{
				page -= s_PageSize;
				if (CommitGuardPage(page))
				{
					return;
				}
			}
		}

		throw std::bad_alloc();
	}

	Stack::~Stack()
	{
		VirtualFree(m_base, m_end - m_base, MEM_RELEASE);
	}

	Result Stack::RegisterStackHandler()
	{
		s_Handler = AddVectoredContinueHandler(0, StackOnSEHExcept);
		return (s_Handler != nullptr) ? R_SUCCESS : R_FAIL;
	}

	Result Stack::UnregisterStackHandler()
	{
		return RemoveVectoredContinueHandler(s_Handler) ? R_SUCCESS : R_FAIL;
	}

	Result Stack::Push(AhaType type, const void* pData)
	{
		size_t size = AhaTypeGetSize(type);
		size_t unit_size = unitof(size);

		aha_u8* sp = m_sp - unit_size - sizeof(unit_t);
		if (sp < m_end)
			return R_STACK_OVERFLOW;

		*(unit_t*)sp = type;
		if (size != unit_size)
			memset(sp + sizeof(unit_t), 0, unit_size);
		memcpy(sp + sizeof(unit_t), pData, size);

		m_sp = sp;
		return R_SUCCESS;
	}

	Result Stack::Pop(AhaType* pType, void* pData)
	{
		aha_u8* sp = m_sp;

		if (sp >= m_bp)
			return R_STACK_UNDERFLOW;

		AhaType type = (AhaType)(*(unit_t*)sp);
		size_t size = AhaTypeGetSize(type);
		size_t unit_size = unitof(size);

		*pType = type;
		sp += sizeof(unit_t);
		memcpy(pData, sp, size);
		sp += unit_size;

		m_sp = sp;
		return R_SUCCESS;
	}
}
