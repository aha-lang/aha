#pragma once

#include <ahabin/ahabindef.h>

#include <unordered_map>

#define BOOST_LOCALE_ENABLE_CHAR32_T
#define BOOST_LOCALE_ENABLE_CHAR16_T
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/functional/hash.hpp>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>

namespace aha
{
	class Module;

	namespace fs = boost::filesystem;
}

namespace std
{
	template<> struct hash<boost::filesystem::path>
	{
		size_t operator()(const boost::filesystem::path& p) const
		{
			return boost::filesystem::hash_value(p);
		}
	};
}

// ModuleManager.cpp
void intrusive_ptr_add_ref(aha::Module *pModule);
void intrusive_ptr_release(aha::Module *pModule);
