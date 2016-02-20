#pragma once

#include "ahabindef.h"

namespace aha
{
	enum AhaAccess : aha_u32
	{
		AHA_PUBLIC,
		AHA_PROTECTED,
		AHA_PROTECTED_INTERNAL,
		AHA_INTERNAL,
		AHA_PRIVATE,
		COUNT_AHA_ACCESS
	};
	inline bool ValidateAhaAccess(AhaAccess i)
	{
		return (i < COUNT_AHA_ACCESS);
	}
}
