#pragma once

#include "aharundef.h"
#include "ModuleManager.h"

namespace aha
{
	class Context : private boost::noncopyable
	{
	private:
		ModuleManager m_ModuleManager;

	public:
		static void InitGC();

		Context();
		~Context();
	};
}
