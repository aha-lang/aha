#pragma once

#include "aharundef.h"

namespace aha
{
	class ModuleManager;

	class Context : private boost::noncopyable
	{
	private:
		std::unique_ptr<ModuleManager> m_pModuleManager;

		std::vector<fs::path> m_FindingPaths;
		fs::path m_CurrentPath;

		void* m_CodeHeap;

	public:
		static void InitGC();

		Context();
		~Context();

		boost::intrusive_ptr<Module> LoadModule(const fs::path& modulePath);

		const std::vector<fs::path> GetFindingPaths() const;
		const fs::path GetCurrentPath() const;
		void* GetCodeHeap() const;
	};
}
