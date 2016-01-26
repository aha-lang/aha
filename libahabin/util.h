#pragma once

#include <stdlib.h>
#include <memory>

namespace util
{
	struct malloc_deleter
	{
		void operator ()(void *ptr) const
		{
			free(ptr);
		}
	};

	template <typename T>
	using malloc_unique_ptr = std::unique_ptr<T, malloc_deleter>;
}
