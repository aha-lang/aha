#include "aharun/Module.h"
#include "aharun/Context.h"
#include "aharun/ModuleManager.h"
#include "aharun/NativeLibrary.h"

namespace aha
{
	fs::path Module::FindModule(const fs::path& path, const std::vector<fs::path>& findingPath,
		const std::string& ext /* = "abf" */)
	{
		boost::system::error_code ec;

		fs::path clone = path;
		if (clone.extension().empty())
			clone.replace_extension(ext);

		if (clone.is_absolute())
		{
			return clone;
		}
		else
		{
			fs::path found;

			for (auto dir : findingPath)
			{
				found = fs::canonical(dir / clone, ec);
				if (!ec && fs::exists(found))
				{
					return found;
				}
			}
		}

		return fs::path();
	}

	Module::Module(const fs::path& path, Context* pContext, ModuleManager* pManager)
		: m_pContext(pContext), m_pManager(pManager), m_refcount(1)
		, m_path(path)
	{
		std::ifstream file(m_path.generic_string(), std::ios::in | std::ios::binary);

		bool f = file.fail();
		bool b = file.bad();
		bool e = file.eof();
		bool g = file.good();

		AhaModule module;
		module.Read(file);

		fs::path moduleDir = path.parent_path();

		for (aha::aha_i32 idx : module.GetRefer().Get())
		{
			const std::u16string& str = module.GetStrings().Get()[idx];
			std::wstring wstr = boost::locale::conv::utf_to_utf<wchar_t>(str);
			fs::path referPath(wstr);
			m_Refers.push_back(m_pManager->LoadModule(referPath, moduleDir));
		}

		for (aha::aha_i32 idx : module.GetNativeRefer().Get())
		{
			const std::u16string& str = module.GetStrings().Get()[idx];
			m_NativeRefers.push_back(NativeLibrary::Load(str, std::vector<fs::path> { m_path.parent_path() }));
		}
	}

	Module::~Module()
	{
	}

	void Module::IncRef()
	{
		m_refcount++;
	}

	void Module::DecRef()
	{
		if (--m_refcount == 0)
		{
			m_pManager->RemoveModule(m_path);
		}
	}
}
