#include <utility>
#include <stdexcept>

#include "aharun/ModuleManager.h"
#include "aharun/Module.h"
#include "aharun/Context.h"
#include "aharun/exceptions.h"

void intrusive_ptr_add_ref(aha::Module *pModule)
{
	pModule->IncRef();
}

void intrusive_ptr_release(aha::Module *pModule)
{
	pModule->DecRef();
}

namespace aha
{
	ModuleManager::ModuleManager(Context *pContext)
		: m_pContext(pContext)
	{
	}

	ModuleManager::~ModuleManager()
	{
	}

	boost::intrusive_ptr<Module> ModuleManager::LoadModule(const fs::path& path, const fs::path& findPath)
	{
		std::vector<fs::path> fpaths;
		if (!findPath.empty())
			fpaths.push_back(findPath);

		auto &vfp = m_pContext->GetFindingPaths();
		fpaths.insert(fpaths.end(), vfp.begin(), vfp.end());

		auto modulePath = Module::FindModule(path, fpaths);
		if (modulePath.empty())
			throw FileNotFoundError();

		auto it = m_modules.find(path);
		if (it == m_modules.end())
		{
			auto pModule = std::make_unique<Module>(modulePath, m_pContext, this);
			auto pRet = boost::intrusive_ptr<Module>(pModule.get(), true);

			m_modules.insert(std::make_pair(path, std::move(pModule)));

			return pRet;
		}
		else
		{
			return boost::intrusive_ptr<Module>(it->second.get(), true);
		}
	}

	void ModuleManager::RemoveModule(const fs::path& path)
	{
		auto it = m_modules.find(path);
		if (it == m_modules.end() || it->second->GetRef() != 0)
			throw std::invalid_argument("path");

		m_modules.erase(it);
	}
}
