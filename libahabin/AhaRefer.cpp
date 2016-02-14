#include "stdafx.h"
#include "ahabin/AhaRefer.h"
#include "ahabin/exceptions.h"

namespace aha
{
	void AhaRefer::Read(aha_i32 SizeOfRefer, std::istream& strm)
	{
		std::vector<aha_i32> refers;

		if (SizeOfRefer % sizeof(aha_i32) != 0)
			throw BadModuleReferError();

		refers.resize(SizeOfRefer / sizeof(aha_i32));

		strm.read((char*)refers.data(), SizeOfRefer);

		/*
		for (ahabin::aha_i32 i = 0; i < refers.Get().GetLength(); ++i)
		{
			// TODO: validate
		}
		*/

		m_refers = std::move(refers);
	}

	const std::vector<aha_i32>& AhaRefer::Get() const
	{
		return m_refers;
	}
}
