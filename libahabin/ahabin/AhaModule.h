#pragma once

#include "ahabindef.h"

#include "AhaStrings.h"
#include "AhaRefer.h"
#include "AhaNativeRefer.h"
#include "AhaBody.h"

namespace aha
{
	struct AhaModuleHeader
	{
		aha_u8 mark[4];
		aha_u32 SizeOfStrings;
		aha_u32 SizeOfRefer;
		aha_u32 SizeOfNativeRefer;
		aha_u32 SizeOfBody;
		aha_u32 ModuleName;
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
		void Read(std::istream& strm);

		AhaStrings& GetStrings();
		AhaRefer& GetRefer();
		AhaNativeRefer& GetNativeRefer();
		AhaBody& GetBody();
		aha_u32 GetModuleName() const;
	};
}
