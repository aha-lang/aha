#pragma once

#include "aharundef.h"
#include <ahabin/AhaModule.h>
#include "aharun/NativeLibrary.h"

namespace aha
{
	class Context;
	class ModuleManager;

	class ClassInfo;

	class Module : private boost::noncopyable
	{
	private:
		Context *m_pContext;

		ModuleManager *m_pManager;
		unsigned m_refcount;

		fs::path m_path;

		std::u16string m_name;

		std::vector<boost::intrusive_ptr<Module>> m_Refers;
		std::vector<std::unique_ptr<NativeLibrary>> m_NativeRefers;

		std::vector<std::unique_ptr<ClassInfo>> m_ClassList;

	public:
		static fs::path FindModule(const fs::path& path, const std::vector<fs::path>& findingPath,
			const std::string& ext = "abf");

		explicit Module(const fs::path& path, Context* pContext, ModuleManager* pManager);
		~Module();

		void IncRef();
		void DecRef();

		unsigned GetRef() const;

		const fs::path& GetPath() const;

		const std::u16string& GetName() const;
		const std::vector<boost::intrusive_ptr<Module>>& GetRefers() const;
		const std::vector<std::unique_ptr<NativeLibrary>>& GetNativeRefers() const;
		const std::vector<std::unique_ptr<ClassInfo>>& GetClassList() const;

		Context* GetContext() const;
	};
}
