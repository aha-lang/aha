#pragma once

#include "ahabindef.h"
#include "AhaAccess.h"
#include "AhaClsMember.h"

namespace aha
{
	enum AhaClassType : aha_i32
	{
		AHA_CLS_NORMAL,
		AHA_CLS_ABSTRACT,
		AHA_CLS_SEALED,
		AHA_CLS_STATIC,
		AHA_CLS_INTERFACE,
		COUNT_AHA_CLS_TYPE
	};

	struct AhaClass_raw
	{
		AhaAccess access;
		AhaClassType type;
		aha_i32 name;
		aha_i32 base;			// 없을 경우 -1
		aha_i32 interfaces;		// "Inter1\0Inter2\0\0"
		aha_i32 CountOfMembers;
		// AhaClsMember members[CountOfMembers];
	};

	class AhaClass
	{
	private:
		AhaClass_raw m_raw;
		std::vector<AhaClsMember> m_members;

	public:
		void Read(std::istream& strm, size_t& read);

		const AhaClass_raw GetRaw() const;
		const std::vector<AhaClsMember>& GetMembers() const;
	};
}
