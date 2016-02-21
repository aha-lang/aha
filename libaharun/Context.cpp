#define GC_THREADS
#include <gc.h>

#include <utility>

#include "aharun/Context.h"

#include "aharun/ModuleManager.h"
#include "aharun/Code.h"

namespace aha
{
	void Context::InitGC()
	{
		GC_INIT();
		GC_set_no_dls(1);
		GC_clear_roots();
		GC_enable_incremental();
	}

	Context::Context()
		: m_CurrentPath(boost::filesystem::current_path())
	{
		const char *fpaths = std::getenv("AHA_FINDING_PATH");
		if (fpaths != nullptr)
		{
			boost::char_separator<char> sep(";");
			boost::tokenizer<boost::char_separator<char>> tok(std::string(fpaths), sep);
			for (auto str : tok)
			{
				boost::filesystem::path p = str;
				if (boost::filesystem::is_directory(p))
					m_FindingPaths.push_back(std::move(p));
			}
		}

		m_CodeHeap = CodeHeap::Create();

		m_pModuleManager = std::make_unique<ModuleManager>(this);
	}

	Context::~Context()
	{
	}

	boost::intrusive_ptr<Module> Context::LoadModule(const boost::filesystem::path& modulePath)
	{
		return m_pModuleManager->LoadModule(modulePath, GetCurrentPath());
	}

	const std::vector<fs::path> Context::GetFindingPaths() const
	{
		return m_FindingPaths;
	}

	const fs::path Context::GetCurrentPath() const
	{
		return m_CurrentPath;
	}

	void* Context::GetCodeHeap() const
	{
		return m_CodeHeap;
	}
}
