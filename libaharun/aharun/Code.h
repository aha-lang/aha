#pragma once

#include "aharundef.h"

namespace aha
{
	class Context;
	class Module;
	class ClassInfo;
	class FunctionInfo;

	class CodeHeap
	{
	public:
		static void* Create();

	private:
		static void* _alloc(void* heap, size_t sz);
		static void _free(void* heap, void* ptr);

		CodeHeap() = delete;
		~CodeHeap() = delete;

	public:
		template <typename T>
		class allocator
		{
			template <typename> friend class allocator;
		private:
			void* m_heap;

		public:
			using value_type = T;
			using propagate_on_container_copy_assignment = std::true_type;
			using propagate_on_container_move_assignment = std::true_type;
			using propagate_on_container_swap = std::true_type;

			explicit allocator(void* heap) noexcept
				: m_heap(heap)
			{
			}

			template <typename U>
			allocator(const allocator<U>& other) noexcept
				: m_heap(other.m_heap)
			{
			}

			T* allocate(size_t n)
			{
				return (T*)_alloc(m_heap, sizeof(T) * n);
			}

			void deallocate(T* ptr, size_t n) noexcept
			{
				_free(m_heap, ptr);
			}

			template <typename U>
			bool operator ==(const allocator<U>& other) noexcept
			{
				return (m_heap == other.m_heap);
			}
			template <typename U>
			bool operator !=(const allocator<U>& other) noexcept
			{
				return !(*this == other);
			}
		};

		template <typename T>
		using vector = std::vector<T, allocator<T>>;
	};

	namespace detail
	{
		class Compiler;
	}

	class Code
	{
		friend detail::Compiler;
	private:
		FunctionInfo* m_pFuncInfo;

		std::vector<aha_u8> m_opcode;
		CodeHeap::vector<aha_u8> m_CompiledCode;

	public:
		explicit Code(FunctionInfo* pFuncInfo);
		~Code();

		void Run();
		void Compile();

	private:
		static void do_jit(Code* pCode);

	public:
		std::vector<aha_u8>& GetOpcode();
		const std::vector<aha_u8>& GetOpcode() const;

		FunctionInfo* GetFunctionInfo() const;
		ClassInfo* GetClassInfo() const;
		Module* GetModule() const;
		Context* GetContext() const;
	};
}
