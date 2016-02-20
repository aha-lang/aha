#pragma once

#include "ahabindef.h"
#include "AhaAccess.h"
#include "AhaClsMember.h"

namespace aha
{
	enum AhaClassType : aha_u32
	{
		AHA_CLS_NORMAL,
		AHA_CLS_ABSTRACT,
		AHA_CLS_SEALED,
		AHA_CLS_STATIC,
		AHA_CLS_INTERFACE,
		COUNT_AHA_CLS_TYPE
	};
	inline bool ValidateAhaClassType(AhaClassType i)
	{
		return (i < COUNT_AHA_CLS_TYPE);
	}

	struct AhaClass_raw
	{
		AhaAccess access;
		AhaClassType type;
		aha_u32 name;
		aha_u32 base;			// base/interfaces가 없을 경우 각각 0xffffffff
		aha_u32 interfaces;		// "Inter1\0Inter2\0\0"
		aha_u32 CountOfMembers;
		// AhaClsMember members[CountOfMembers];
	};

	class AhaStrings;

	class AhaClass
	{
	private:
		AhaClass_raw m_raw;
		std::vector<AhaClsMember> m_members;

	public:
		void Read(std::istream& strm, size_t& read);
		void Validate(const AhaStrings& strings);

		const AhaClass_raw GetRaw() const;
		const std::vector<AhaClsMember>& GetMembers() const;
	};
}
