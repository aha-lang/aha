#pragma once

#include "aharundef.h"
#include <ahabin/AhaType.h>

namespace aha
{
	class ClassInfo;
	class Module;

	struct TypeVar
	{
		union
		{
			AhaType type;
			ClassInfo* pClsInfo;
		};
		bool bPrimitive;

		void Init(AhaType t, Module* pModule);
	};
}
