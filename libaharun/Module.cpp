#include "aharun/Module.h"

#include "aharun/ModuleManager.h"
#include "aharun/NativeLibrary.h"
#include "aharun/ClassInfo.h"

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

		m_name = module.GetStrings().Get()[module.GetModuleName()];

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

		for (const AhaClass& cls : module.GetBody().Get())
		{
			m_ClassList.push_back(std::make_unique<ClassInfo>(cls, module.GetStrings(), this));
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

	unsigned Module::GetRef() const
	{
		return m_refcount;
	}

	const fs::path& Module::GetPath() const
	{
		return m_path;
	}

	const std::u16string& Module::GetName() const
	{
		return m_name;
	}

	const std::vector<boost::intrusive_ptr<Module>>& Module::GetRefers() const
	{
		return m_Refers;
	}
	const std::vector<std::unique_ptr<NativeLibrary>>& Module::GetNativeRefers() const
	{
		return m_NativeRefers;
	}

	const std::vector<std::unique_ptr<ClassInfo>>& Module::GetClassList() const
	{
		return m_ClassList;
	}

	Context* Module::GetContext() const
	{
		return m_pContext;
	}

}
