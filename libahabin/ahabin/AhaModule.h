#pragma once

#include "ahabindef.h"
#include "Result.h"

#include "AhaStrings.h"
#include "AhaRefer.h"
#include "AhaNativeRefer.h"
#include "AhaBody.h"

namespace ahabin
{
	class ReadStream;

	struct AhaModuleHeader
	{
		aha_byte mark[4];
		aha_i32 SizeOfStrings;
		aha_i32 SizeOfRefer;
		aha_i32 SizeOfNativeRefer;
		aha_i32 SizeOfBody;
		aha_i32 ModuleName;
	};

	class AhaModule
	{
	private:
		AhaModuleHeader m_Header;
		AhaStrings m_Strings;
		AhaRefer m_Refer;
		AhaNativeRefer m_NativeRefer;
		AhaBody m_Body;

	public:
		Result Read(ReadStream& strm);

		AhaStrings& GetStrings();
		AhaRefer& GetRefer();
		AhaNativeRefer& GetNativeRefer();
		AhaBody& GetBody();
	};
}
