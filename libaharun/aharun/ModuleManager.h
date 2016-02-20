#pragma once

#include "aharundef.h"

namespace aha
{
	class Context;
	class Module;

	class ModuleManager : private boost::noncopyable
	{
	private:
		Context *m_pContext;

		std::unordered_map<fs::path, std::unique_ptr<Module>> m_modules;

	public:
		explicit ModuleManager(Context *pContext);
		~ModuleManager();

		boost::intrusive_ptr<Module> LoadModule(const fs::path& path, const fs::path& findPath);
		void RemoveModule(const fs::path& path);
	};
}
