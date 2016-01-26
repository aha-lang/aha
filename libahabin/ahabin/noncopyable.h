#pragma once

#include "ahabindef.h"

namespace ahabin
{
	class noncopyable
	{
	private:
		noncopyable(const noncopyable&) = delete;
		noncopyable& operator =(const noncopyable&) = delete;
	protected:
		noncopyable() = default;
	};
}
