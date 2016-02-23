#include "aharun/TypeVar.h"
#include "aharun/Module.h"

namespace aha
{
	void TypeVar::Init(AhaType t, Module* pModule)
	{
		type = t;

		if (type & aha::AHA_TYPE_FLG_PRIMITIVE)
		{
			bPrimitive = true;
		}
		else
		{
			bPrimitive = false;
			// TODO
		}
	}
}
