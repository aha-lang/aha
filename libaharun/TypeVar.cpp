#include "aharun/TypeVar.h"
#include "aharun/Module.h"

namespace aha
{
	void TypeVar::Init(AhaType t, Module* pModule)
	{
		type = t;

		if (type & 0x80000000)
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
