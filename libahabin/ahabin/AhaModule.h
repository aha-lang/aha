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
		void Read(std::istream& strm, bool bValidate = true);
		void Write(std::ostream& strm, bool bValidate = true);
		void Validate() const;

		AhaModuleHeader& GetHeader();
		const AhaModuleHeader& GetHeader() const;

		AhaStrings& GetStrings();
		const AhaStrings& GetStrings() const;

		AhaRefer& GetRefer();
		const AhaRefer& GetRefer() const;

		AhaNativeRefer& GetNativeRefer();
		const AhaNativeRefer& GetNativeRefer() const;

		AhaBody& GetBody();
		const AhaBody& GetBody() const;

		aha_u32 GetModuleName() const;
	};
}
