#pragma once

#include "aharundef.h"

namespace aha
{
	class Context;

	class Thread : private boost::noncopyable
	{
	private:
		Context *m_pContext;

	public:
		explicit Thread(Context *m_pContext);
		~Thread();
	};
}
