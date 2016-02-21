#pragma once

#include "aharundef.h"
#include <ahabin/AhaClsMember.h>

#include "TypeVar.h"
#include "Code.h"

namespace aha
{
	class AhaStrings;

	class Context;
	class Module;
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

		Code m_code;

	public:
		explicit FunctionInfo(const AhaClsMember& clsmem, const AhaStrings& strings, ClassInfo *pClassInfo);
		~FunctionInfo();

		void Call(void* thiz, void** param);

		const std::u16string& GetName() const;

		AhaAccess GetAccess() const;
		AhaClsMemberStorage GetStorage() const;
		const TypeVar& GetRetType() const;
		const std::vector<AhaType> GetParams() const;

		Code& GetCode();
		const Code& GetCode() const;

		ClassInfo* GetClassInfo() const;
		Module* GetModule() const;
		Context* GetContext() const;

		bool operator <(const FunctionInfo& rhs) const
		{
			return (m_name < rhs.m_name);
		}
	};
}
