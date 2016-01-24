#pragma once

#include "ahabindef.h"
#include "Result.h"

namespace ahabin
{
	template <typename T>
	class ArrayList
	{
	private:
		T* m_ar;
		size_t m_length;
		size_t m_capacity;

		ArrayList(const ArrayList&) = delete;
		ArrayList &operator =(const ArrayList&) = delete;

	public:
		ArrayList()
		{
			m_ar = nullptr;
			m_length = 0;
			m_capacity = 0;
		}

		~ArrayList()
		{
			if (m_ar != nullptr)
			{
				for (size_t i = 0; i < m_length; ++i)
					m_ar[i].~T();

				free(m_ar);
			}
		}

		ArrayList(ArrayList&& other)
		{
			m_ar = other.m_ar;
			m_length = other.m_length;
			m_capacity = other.m_capacity;
			other.m_ar = nullptr;
			other.m_length = 0;
			other.m_capacity = 0;
		}

		ArrayList& operator =(ArrayList&& other)
		{
			ArrayList(std::move(other)).swap(*this);
			return *this;
		}

		void swap(ArrayList& other)
		{
			std::swap(m_ar, other.m_ar);
			std::swap(m_length, other.m_length);
			std::swap(m_capacity, other.m_capacity);
		}

		size_t GetLength() const
		{
			return m_length;
		}

		T* data() const
		{
			return m_ar;
		}

		T& operator [](size_t idx)
		{
			return const_cast<T&>(static_cast<const ArrayList&>(*this)[idx]);
		}
		const T& operator [](size_t idx) const
		{
			return m_ar[idx];
		}

		Result Reserve(size_t capacity)
		{
			if (capacity == 0)
				return R_INVALID_OPERATION;

			size_t bits;
			for (bits = 1; capacity >> bits != 0; ++bits) { }
			if (capacity != (1 << (bits - 1)))
				capacity = 1 << bits;

			if (capacity < m_length)
			{
				return R_INVALID_OPERATION;
			}
			else if (m_ar == nullptr)
			{
				m_ar = (T*)malloc(sizeof(T) * capacity);
				if (m_ar == nullptr)
					return R_OUT_OF_MEMORY;
				m_capacity = capacity;
				return R_SUCCESS;
			}
			else
			{
				T* newar = (T*)malloc(sizeof(T) * capacity);
				if (newar == nullptr)
					return R_OUT_OF_MEMORY;

				for (size_t i = 0; i < m_length; ++i)
				{
					new (&newar[i]) T(std::move(m_ar[i]));
					m_ar[i].~T();
				}

				free(m_ar);
				m_ar = newar;
				m_capacity = capacity;
				return R_SUCCESS;
			}
		}

		template <typename U>
		Result Relength(size_t length, U&& data)
		{
			Result rs;

			if (length > m_capacity)
			{
				if (RESULT_FAIL(rs = Reserve(length)))
					return rs;
			}

			for (size_t i = length; i < m_length; ++i)
				m_ar[i].~T();

			for (size_t i = m_length; i < length; ++i)
				new (&m_ar[i]) T(std::forward<U>(data));

			m_length = length;
			return R_SUCCESS;
		}
		Result Relength(size_t length)
		{
			return Relength(length, T());
		}

		template <typename U>
		Result PushBack(U&& data)
		{
			Result rs;

			if (m_length + 1 > m_capacity)
			{
				if (RESULT_FAIL(rs = Reserve(m_length + 1)))
					return rs;
			}
			m_length++;

			new (&m_ar[m_length - 1]) T(std::forward<U>(data));
			return R_SUCCESS;
		}
	};
}
