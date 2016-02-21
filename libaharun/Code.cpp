#include "aharun/Code.h"

#include "aharun/FunctionInfo.h"
#include "aharun/ClassInfo.h"
#include "aharun/Module.h"
#include "aharun/Context.h"

#include <Windows.h>
#ifdef GetClassInfo
#undef GetClassInfo
#endif

namespace
{
	template <typename T, typename C>
	void push(C& vt, const T& data)
	{
		const char* src = (const char*)&data;
		for (int i = 0; i < sizeof(T); ++i)
			vt.push_back(*src++);
	}

	template <typename C>
	void push_rel(C& vt, void* addr)
	{
		uintptr_t val = (uintptr_t)addr - ((uintptr_t)vt.data() + vt.size());
		push<uintptr_t>(vt, val);
	}
}

namespace aha
{
	void* CodeHeap::Create()
	{
		return (void*)HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
	}
	void* CodeHeap::_alloc(void* heap, size_t sz)
	{
		return HeapAlloc((HANDLE)heap, 0, sz);
	}
	void CodeHeap::_free(void* heap, void* ptr)
	{
		HeapFree(heap, 0, ptr);
	}

	Code::Code(FunctionInfo* pFuncInfo)
		: m_pFuncInfo(pFuncInfo)
		, m_CompiledCode(CodeHeap::allocator<aha_u8>(pFuncInfo->GetContext()->GetCodeHeap()))
	{
		m_CompiledCode.reserve(64);

		push<aha_u8>(m_CompiledCode, 0x68); // push imm32
		push<aha_uptr>(m_CompiledCode, (aha_uptr)this);
		push<aha_u8>(m_CompiledCode, 0xe8); // call near
		push_rel(m_CompiledCode, &Code::do_jit);
	}

	Code::~Code()
	{
	}

	void Code::Run()
	{
		void* c = m_CompiledCode.data();
		__asm
		{
			call c
		}
	}

	void Code::do_jit(Code* pCode)
	{
		pCode->Compile();

		void* c = pCode->m_CompiledCode.data();
		__asm
		{
			leave
			leave
			jmp c
		}
	}

	std::vector<aha_u8>& Code::GetOpcode()
	{
		return m_opcode;
	}

	const std::vector<aha_u8>& Code::GetOpcode() const
	{
		return m_opcode;
	}

	FunctionInfo* Code::GetFunctionInfo() const
	{
		return m_pFuncInfo;
	}

	ClassInfo* Code::GetClassInfo() const
	{
		return GetFunctionInfo()->GetClassInfo();
	}

	Module* Code::GetModule() const
	{
		return GetClassInfo()->GetModule();
	}

	Context* Code::GetContext() const
	{
		return GetModule()->GetContext();
	}
}
