#pragma once

#include "aharundef.h"
#include <ahabin/AhaType.h>

namespace aha
{
	class ClassInfo;

	struct TypeVar
	{
		union
		{
			AhaType type;
			ClassInfo* pClsInfo;
		};
		bool bPrimitive;
	};
}
