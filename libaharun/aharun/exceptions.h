#pragma once

#include "aharundef.h"

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

	DEFINE_EXCEPT(FileNotFoundError, std::runtime_error, "file not found")
	DEFINE_EXCEPT(NativeLibraryLoadError, std::runtime_error, "cannot load native library")

#undef DEFINE_EXCEPT
}
