#pragma once

#include "ahabindef.h"

namespace aha
{
	enum AhaAccess : aha_i32
	{
		AHA_PUBLIC,
		AHA_PROTECTED,
		AHA_PROTECTED_INTERNAL,
		AHA_INTERNAL,
		AHA_PRIVATE,
		AHA_ACCESS_SIZE
	};
}
