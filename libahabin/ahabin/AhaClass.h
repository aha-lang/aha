#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "AhaAccess.h"
#include "AhaClsMember.h"
#include "ArrayList.h"

namespace ahabin
{
	class ReadStream;

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
		size_t SizeOfMembers;
		// AhaClsMember members[SizeOfMembers];
	};

	class AhaClass
	{
	private:
		AhaClass_raw m_raw;
		ArrayList<AhaClsMember> m_members;

	public:
		static Result Create(ReadStream& strm, AhaClass& obj);
	};
}
