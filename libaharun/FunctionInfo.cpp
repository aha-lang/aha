#include "aharun/FunctionInfo.h"

#include "aharun/ClassInfo.h"
#include "aharun/Module.h"

#include <ahabin/AhaStrings.h>

namespace aha
{
	FunctionInfo::FunctionInfo(const AhaClsMember& clsmem, const AhaStrings& strings, ClassInfo* pClassInfo)
		: m_pClassInfo(pClassInfo)
		, m_code(this)
	{
		m_name = strings.Get()[clsmem.GetRaw().name];
		m_access = clsmem.GetRaw().access;
		m_storage = clsmem.GetRaw().storage;

		m_rettype.Init(clsmem.GetRaw().function.rettype, pClassInfo->GetModule());

		m_params = clsmem.GetParams();
		m_code.GetOpcode() = clsmem.GetOpcode();
	}

	FunctionInfo::~FunctionInfo()
	{
	}

	void FunctionInfo::Call(void* thiz, void** param)
	{
		;
	}

	const std::u16string& FunctionInfo::GetName() const
	{
		return m_name;
	}

	AhaAccess FunctionInfo::GetAccess() const
	{
		return m_access;
	}

	AhaClsMemberStorage FunctionInfo::GetStorage() const
	{
		return m_storage;
	}

	const TypeVar& FunctionInfo::GetRetType() const
	{
		return m_rettype;
	}

	const std::vector<AhaType> FunctionInfo::GetParams() const
	{
		return m_params;
	}

	Code& FunctionInfo::GetCode()
	{
		return m_code;
	}

	const Code& FunctionInfo::GetCode() const
	{
		return m_code;
	}

	ClassInfo* FunctionInfo::GetClassInfo() const
	{
		return m_pClassInfo;
	}

	Module* FunctionInfo::GetModule() const
	{
		return GetClassInfo()->GetModule();
	}

	Context* FunctionInfo::GetContext() const
	{
		return GetModule()->GetContext();
	}
}
