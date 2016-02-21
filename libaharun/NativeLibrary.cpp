#include <locale>

#include "aharun/NativeLibrary.h"
#include "aharun/Module.h"
#include "aharun/exceptions.h"

#include <Windows.h>
#ifdef GetClassInfo
#undef GetClassInfo
#endif

namespace aha
{
	NativeLibrary::NativeLibrary()
	{

	}

	NativeLibrary::~NativeLibrary()
	{
		HMODULE hDLL = (HMODULE)m_handle;
		FreeLibrary(hDLL);
	}

	std::unique_ptr<NativeLibrary> NativeLibrary::Load(const std::u16string& path, const std::vector<fs::path>& findingPaths)
	{
		std::wstring wp = boost::locale::conv::utf_to_utf<wchar_t>(path);
		HMODULE hDLL = LoadLibraryW(wp.c_str());
		if (hDLL == nullptr)
		{
			if (GetLastError() == ERROR_MOD_NOT_FOUND && boost::filesystem::path(wp).is_relative())
			{
				auto modulePath = Module::FindModule(wp, findingPaths, "dll");
				return Load(boost::locale::conv::utf_to_utf<char16_t>(modulePath.make_preferred().wstring()), findingPaths);
			}

			wchar_t *buf;
			FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				nullptr, 0, 0, (wchar_t *)&buf, 0, nullptr);
			std::string str = boost::locale::conv::from_utf<wchar_t>(buf, std::locale());
			LocalFree(buf);

			throw NativeLibraryLoadError(str);
		}

		auto ret = std::unique_ptr<NativeLibrary>(new NativeLibrary());
		ret->m_handle = hDLL;
		return ret;
	}

	void* NativeLibrary::GetProc(const std::u16string& str)
	{
		HMODULE hDLL = (HMODULE)m_handle;
		std::string astr = boost::locale::conv::from_utf<char16_t>(str, std::locale());
		return GetProcAddress(hDLL, astr.c_str());
	}
}
