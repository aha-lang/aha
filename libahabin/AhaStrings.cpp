#include "ahabin/AhaStrings.h"
#include "ahabin/exceptions.h"
#include "util.h"

namespace aha
{
	void AhaStrings::Read(aha_i32 SizeOfStrings, std::istream& strm)
	{
		std::vector<std::u16string> strings;
		char padding[4];

		size_t read = 0, szofstr = (size_t)SizeOfStrings;

		while (read < szofstr)
		{
			std::u16string str;
			aha_i32 size;

			read += sizeof(size);
			if (read >= szofstr)
				throw BadModuleStringsError();
			strm.read((char*)&size, sizeof(size));

			if (size % sizeof(aha_i16) != 0)
				return throw BadModuleStringsError();

			read += size;
			if (read > szofstr)
				return throw BadModuleStringsError();

			str.resize(size / sizeof(char16_t));
			strm.read((char*)&str[0], size);

			if (size % 4 != 0)
			{
				size_t padding_sz = 4 - size % 4;
				strm.read(padding, padding_sz);
				read += padding_sz;
			}

			strings.push_back(std::move(str));
		}

		if (read != szofstr)
			return throw BadModuleStringsError();

		m_strings = std::move(strings);
	}

	const std::vector<std::u16string>& AhaStrings::Get() const
	{
		return m_strings;
	}
}
