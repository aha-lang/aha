#pragma once

#include "aharundef.h"
#include <ahabin/AhaClsMember.h>

#include "TypeVar.h"

namespace aha
{
	class AhaStrings;
	class ClassInfo;

	class FunctionInfo : private boost::noncopyable
	{
	private:
		ClassInfo *m_pClassInfo;

		std::u16string m_name;

		AhaAccess m_access;
		AhaClsMemberStorage m_storage;

		TypeVar m_rettype;

		std::vector<AhaType> m_params;
		std::vector<aha_u8> m_opcode;

	public:
		explicit FunctionInfo(const AhaClsMember& clsmem, const AhaStrings& strings, ClassInfo *pClassInfo);
		~FunctionInfo();

		const std::u16string& GetName() const
		{
			return m_name;
		}

		AhaAccess GetAccess() const
		{
			return m_access;
		}
		AhaClsMemberStorage GetStorage() const
		{
			return m_storage;
		}
		const TypeVar& GetTypeVar() const
		{
			return m_rettype;
		}

		ClassInfo* GetClassInfo() const
		{
			return m_pClassInfo;
		}

		bool operator <(const FunctionInfo& rhs) const
		{
			return (m_name < rhs.m_name);
		}
	};
}
