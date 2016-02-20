#pragma once

#include "aharundef.h"
#include <ahabin/AhaModule.h>
#include "aharun/NativeLibrary.h"

namespace aha
{
	class Context;
	class ModuleManager;

	class Module : private boost::noncopyable
	{
	private:
		Context *m_pContext;
		ModuleManager *m_pManager;
		unsigned m_refcount;

		fs::path m_path;

		std::vector<boost::intrusive_ptr<Module>> m_Refers;
		std::vector<std::unique_ptr<NativeLibrary>> m_NativeRefers;

	public:
		static fs::path FindModule(const fs::path& path, const std::vector<fs::path>& findingPath,
			const std::string& ext = "abf");

		explicit Module(const fs::path& path, Context* pContext, ModuleManager* pManager);
		~Module();

		void IncRef();
		void DecRef();

		unsigned GetRef() const
		{
			return m_refcount;
		}

		const fs::path& GetPath() const
		{
			return m_path;
		}
	};
}
