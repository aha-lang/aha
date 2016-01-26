#pragma once

#include "aharundef.h"
#include <ahabin/noncopyable.h>
#include "ModuleManager.h"
#include "Stack.h"

namespace aha
{
	class Context : private noncopyable
	{
	private:
		ModuleManager m_ModuleManager;
		Stack m_Stack;

	public:
		Context();
		~Context();
	};
}
