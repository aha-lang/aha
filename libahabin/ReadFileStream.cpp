#include "stdafx.h"
#include "ahabin/ReadFileStream.h"

namespace ahabin
{
	Result ReadFileStream::Create(const char* filename)
	{
		if (fopen_s(&m_fp, filename, "rb") != 0)
			return R_IO_ERROR;

		return R_SUCCESS;
	}

	Result ReadFileStream::Read(void* buf, size_t size)
	{
		if (size == 0)
			return R_SUCCESS;

		if (fread(buf, size, 1, m_fp) != 1)
		{
			if (feof(m_fp))
				return R_END_OF_FILE;
			else
				return R_IO_ERROR;
		}

		return R_SUCCESS;
	}

	bool ReadFileStream::EndOfFile()
	{
		return feof(m_fp) ? true : false;
	}
}
