#include <new>

#include "aharun/ClassInfo.h"
#include <ahabin/AhaStrings.h>

namespace aha
{
	ClassInfo::ClassInfo(AhaClass* pClass, AhaStrings* pStrings, Context* pContext, Module* pModule)
		: m_pContext(pContext), m_pModule(pModule)
	{
		m_name = pStrings->Get()[pClass->GetRaw().name];
		m_access = pClass->GetRaw().access;
		m_ClassType = pClass->GetRaw().type;

		std::vector<uint8_t> InsVarInitial, ClsVarTable;

		for (auto& member : pClass->GetMembers())
		{
			VariableInfo vi;

			if (member.GetRaw().type == AHA_CLSMEM_TYPE_VAR)
			{
				vi.name = pStrings->Get()[member.GetRaw().name];
				vi.access = member.GetRaw().access;
				vi.type.type = member.GetRaw().variable.vartype;
				
				if (vi.type.type & 0x80000000u)
				{
					vi.type.bPrimitive = true;
				}
				else
				{
					vi.type.bPrimitive = false;
					// TODO
				}

				size_t sz = AhaTypeGetSize(vi.type.type);

				if (member.GetRaw().storage == AHA_CLSMEM_STORAGE_CLASSOF)
				{
					size_t idx = ClsVarTable.size();
					ClsVarTable.resize(idx + sz);
					memcpy(&ClsVarTable[idx], &member.GetRaw().variable.initial, sz);
				}
				else
				{
					size_t idx = InsVarInitial.size();
					InsVarInitial.resize(idx + sz);
					memcpy(&InsVarInitial[idx], &member.GetRaw().variable.initial, sz);
				}
			}
			else
			{
				// TODO
			}
		}

		m_InsVarSize = InsVarInitial.size();
		m_InsVarInitial.reset(malloc(m_InsVarSize));
		if (m_InsVarInitial.get() == nullptr)
			throw std::bad_alloc();
		memcpy(m_InsVarInitial.get(), &InsVarInitial[0], m_InsVarSize);

		m_ClsVarSize = ClsVarTable.size();
		m_ClsVarTable.reset(malloc(m_ClsVarSize));
		if (m_ClsVarTable.get() == nullptr)
			throw std::bad_alloc();
		memcpy(m_ClsVarTable.get(), &ClsVarTable[0], m_ClsVarSize);
	}

	ClassInfo::~ClassInfo()
	{
	}
}
