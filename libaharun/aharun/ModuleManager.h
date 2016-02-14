#pragma once

#include "aharundef.h"

namespace aha
{
	class Module;

	class ModuleManager : private boost::noncopyable
	{
	private:
		std::unordered_map<std::u16string, std::unique_ptr<Module>> m_modules;

	public:
		ModuleManager();
		~ModuleManager();

		void RemoveModule(const std::u16string& path);
	};
}
