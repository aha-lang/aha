#pragma once

#include "aharundef.h"
#include <ahabin/AhaModule.h>

namespace aha
{
	class ModuleManager;

	class Module : private noncopyable
	{
	private:
		ModuleManager *m_pManager;
		unsigned m_refcount;

		std::u16string m_path;

		AhaModule m_module;

	public:
		explicit Module(std::u16string path, ModuleManager *pManager);
		~Module();

		void IncRef();
		void DecRef();
	};
}
