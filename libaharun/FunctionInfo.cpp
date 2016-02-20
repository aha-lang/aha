#include "aharun/FunctionInfo.h"
#include "aharun/ClassInfo.h"

#include <ahabin/AhaStrings.h>

namespace aha
{
	FunctionInfo::FunctionInfo(const AhaClsMember& clsmem, const AhaStrings& strings, ClassInfo* pClassInfo)
		: m_pClassInfo(pClassInfo)
	{
		m_name = strings.Get()[clsmem.GetRaw().name];
		m_access = clsmem.GetRaw().access;
		m_storage = clsmem.GetRaw().storage;

		m_rettype.Init(clsmem.GetRaw().function.rettype, pClassInfo->GetModule());

		m_params = clsmem.GetParams();
		m_opcode = clsmem.GetOpcode();
	}

	FunctionInfo::~FunctionInfo()
	{
	}
}
