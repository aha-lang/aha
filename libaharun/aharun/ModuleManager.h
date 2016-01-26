#pragma once

#include "aharundef.h"
#include <ahabin/noncopyable.h>

namespace aha
{
	class Module;

	class ModuleManager : private noncopyable
	{
	private:
		std::unordered_map<std::u16string, std::unique_ptr<Module>> m_modules;

	public:
		ModuleManager();
		~ModuleManager();

		void FreeModule(const std::u16string& path);
	};
}
