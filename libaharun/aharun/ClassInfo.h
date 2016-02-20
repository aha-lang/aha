#pragma once

#include "aharundef.h"

#include <ahabin/AhaClass.h>
#include <ahabin/util.h>

#include "TypeVar.h"

namespace aha
{
	class AhaStrings;
	class Module;

	class FunctionInfo;

	class ClassInfo : private boost::noncopyable
	{
	public:
		struct VariableInfo
		{
			std::u16string name;

			TypeVar type;
			AhaAccess access;
			unsigned offset;

			bool operator <(const VariableInfo& rhs) const
			{
				return (name < rhs.name);
			}
		};

	private:
		Module* m_pModule;

		std::u16string m_name;

		AhaAccess m_access;
		AhaClassType m_ClassType;

		std::vector<std::unique_ptr<FunctionInfo>> m_FunctionList;

		std::vector<VariableInfo> m_ClsVarInfo;
		malloc_unique_ptr<void> m_ClsVarTable;
		size_t m_ClsVarSize;

		std::vector<VariableInfo> m_InsVarInfo;
		malloc_unique_ptr<void> m_InsVarInitial;
		size_t m_InsVarSize;
		bool m_bAtomicAllocable;

	public:
		explicit ClassInfo(const AhaClass& cls, const AhaStrings& strings, Module* pModule);
		~ClassInfo();

		const VariableInfo* FindClsVariable(const std::u16string& name);
		const VariableInfo* FindInsVariable(const std::u16string& name);

		const std::u16string& GetName() const
		{
			return m_name;
		}
		AhaAccess GetAccess() const
		{
			return m_access;
		}
		AhaClassType GetClassType() const
		{
			return m_ClassType;
		}
		const std::vector<std::unique_ptr<FunctionInfo>>& GetFunctionList() const
		{
			return m_FunctionList;
		}

		Module* GetModule() const
		{
			return m_pModule;
		}
	};
}
