#include <algorithm>
#include <new>

#include "aharun/ClassInfo.h"

#include "aharun/FunctionInfo.h"
#include "aharun/Module.h"

#include <ahabin/AhaStrings.h>

namespace aha
{
	ClassInfo::ClassInfo(const AhaClass& cls, const AhaStrings& strings, Module* pModule)
		: m_pModule(pModule)
	{
		m_name = strings.Get()[cls.GetRaw().name];
		m_access = cls.GetRaw().access;
		m_ClassType = cls.GetRaw().type;

		std::vector<uint8_t> InsVarInitial, ClsVarTable;
		unsigned ins_idx = 0, cls_idx = 0;

		for (const AhaClsMember& member : cls.GetMembers())
		{
			if (member.GetRaw().type == AHA_CLSMEM_TYPE_VAR)
			{
				VariableInfo vi;

				vi.access = member.GetRaw().access;
				vi.name = strings.Get()[member.GetRaw().name];

				vi.type.Init(member.GetRaw().variable.vartype, pModule);

				unsigned sz = AhaTypeGetSize(vi.type.type);

				if (member.GetRaw().storage == AHA_CLSMEM_STORAGE_CLASSOF)
				{
					vi.offset = cls_idx;
					cls_idx += sz;

					unsigned idx = ClsVarTable.size();
					ClsVarTable.resize(idx + sz);
					memcpy(&ClsVarTable[idx], &member.GetRaw().variable.initial, sz);

					m_ClsVarInfo.push_back(vi);
				}
				else
				{
					vi.offset = ins_idx;
					ins_idx += sz;

					size_t idx = InsVarInitial.size();
					InsVarInitial.resize(idx + sz);
					memcpy(&InsVarInitial[idx], &member.GetRaw().variable.initial, sz);

					m_InsVarInfo.push_back(vi);
				}
			}
			else
			{
				m_FunctionList.push_back(std::make_unique<FunctionInfo>(member, strings, this));
			}
		}

		std::sort(m_ClsVarInfo.begin(), m_ClsVarInfo.end());
		std::sort(m_InsVarInfo.begin(), m_InsVarInfo.end());

		m_ClsVarSize = ClsVarTable.size();
		if (m_ClsVarSize != 0)
		{
			m_ClsVarTable.reset(malloc(m_ClsVarSize));
			if (m_ClsVarTable.get() == nullptr)
				throw std::bad_alloc();
			memcpy(m_ClsVarTable.get(), &ClsVarTable[0], m_ClsVarSize);
		}

		m_InsVarSize = InsVarInitial.size();
		if (m_InsVarSize != 0)
		{
			m_InsVarInitial.reset(malloc(m_InsVarSize));
			if (m_InsVarInitial.get() == nullptr)
				throw std::bad_alloc();
			memcpy(m_InsVarInitial.get(), &InsVarInitial[0], m_InsVarSize);
		}
	}

	ClassInfo::~ClassInfo()
	{
	}

	const ClassInfo::VariableInfo* ClassInfo::FindClsVariable(const std::u16string& name)
	{
		if (m_ClsVarInfo.empty())
			return nullptr;

		VariableInfo vi;
		vi.name = name;

		auto it = std::lower_bound(m_ClsVarInfo.begin(), m_ClsVarInfo.end(), vi);
		if (it->name == name)
		{
			return std::addressof(*it);
		}
		else
		{
			return nullptr;
		}
	}

	const ClassInfo::VariableInfo* ClassInfo::FindInsVariable(const std::u16string& name)
	{
		if (m_InsVarInfo.empty())
			return nullptr;

		VariableInfo vi;
		vi.name = name;

		auto it = std::lower_bound(m_InsVarInfo.begin(), m_InsVarInfo.end(), vi);
		if (it->name == name)
		{
			return std::addressof(*it);
		}
		else
		{
			return nullptr;
		}
	}

	const std::u16string& ClassInfo::GetName() const
	{
		return m_name;
	}

	AhaAccess ClassInfo::GetAccess() const
	{
		return m_access;
	}

	AhaClassType ClassInfo::GetClassType() const
	{
		return m_ClassType;
	}

	const std::vector<std::unique_ptr<FunctionInfo>>& ClassInfo::GetFunctionList() const
	{
		return m_FunctionList;
	}

	Module* ClassInfo::GetModule() const
	{
		return m_pModule;
	}

	Context* ClassInfo::GetContext() const
	{
		return GetModule()->GetContext();
	}
}
