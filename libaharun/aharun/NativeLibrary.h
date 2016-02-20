#pragma once

#include "aharundef.h"

namespace aha
{
	class NativeLibrary : private boost::noncopyable
	{
	private:
		void *m_handle;

		NativeLibrary();

	public:
		~NativeLibrary();

		static std::unique_ptr<NativeLibrary> Load(const std::u16string& path, const std::vector<fs::path>& findingPaths);

		void* GetProc(const std::u16string& str);
	};
}
