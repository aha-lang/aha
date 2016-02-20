#pragma once

#include "ahabindef.h"

namespace aha
{
#define DEFINE_EXCEPT(name, base, msg) \
	class name : public base \
	{ \
	public: \
		name() : base(msg) { } \
		explicit name(const char* message) : base(message) { } \
		explicit name(const std::string& message) : base(message) { } \
	};

	DEFINE_EXCEPT(BadModuleError, std::runtime_error, "bad abf file")
	DEFINE_EXCEPT(BadModuleHeaderError, BadModuleError, "bad header of abf file")
	DEFINE_EXCEPT(BadModuleStringsError, BadModuleError, "bad strings of abf file")
	DEFINE_EXCEPT(BadModuleReferError, BadModuleError, "bad refer of abf file")
	DEFINE_EXCEPT(BadModuleNativeReferError, BadModuleError, "bad native refer of abf file")
	DEFINE_EXCEPT(BadModuleBodyError, BadModuleError, "bad body of abf file")

#undef DEFINE_EXCEPT
}
