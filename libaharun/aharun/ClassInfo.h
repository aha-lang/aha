#pragma once

#include "aharundef.h"

#include <ahabin/AhaClass.h>
#include <ahabin/util.h>

#include "TypeVar.h"

namespace aha
{
	class AhaStrings;

	class Context;
	class Module;

	class ClassInfo
	{
	public:
		struct VariableInfo
		{
			std::u16string name;
			AhaAccess access;

			TypeVar type;
			uint32_t offset;
		};

	private:
		Context* m_pContext;
		Module* m_pModule;

		std::u16string m_name;

		AhaAccess m_access;
		AhaClassType m_ClassType;

		std::vector<VariableInfo> m_ClsVarInfo;
		malloc_unique_ptr<void> m_ClsVarTable;
		size_t m_ClsVarSize;

		std::vector<VariableInfo> m_InsVarInfo;
		malloc_unique_ptr<void> m_InsVarInitial;
		size_t m_InsVarSize;
		bool m_bAtomicAllocable;

	public:
		explicit ClassInfo(const AhaClass& cls, const AhaStrings& strings, Context* pContext, Module* pModule);
		~ClassInfo();
	};
}
