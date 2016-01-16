#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "ReadStream.h"

namespace ahabin
{
	class StringUTF16
	{
	private:
		aha_u16 *m_str;
		aha_i32 m_length;

		StringUTF16(const StringUTF16&) = delete;
		StringUTF16 &operator =(const StringUTF16&) = delete;

	public:
		StringUTF16();
		~StringUTF16();

		StringUTF16(StringUTF16&& other);
		StringUTF16& operator =(StringUTF16&& other);
		void swap(StringUTF16& other);

		static Result Create(const aha_u16* str, StringUTF16& obj);
		static Result Create(ReadStream& strm, aha_i32 length, StringUTF16& obj);

		aha_i32 GetLength() const;

		aha_u16& operator [](aha_i32 idx);
		const aha_u16& operator [](aha_i32 idx) const;
	};
}
