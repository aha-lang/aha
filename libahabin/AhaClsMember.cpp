#include "ahabin/AhaClsMember.h"
#include "ahabin/AhaStrings.h"
#include "ahabin/exceptions.h"

namespace aha
{
	void AhaClsMember::Read(std::istream& strm, size_t& read)
	{
		char padding[4];

		m_params.resize(0);
		m_opcode.resize(0);

		strm.read((char*)&m_raw, sizeof(m_raw));
		read += sizeof(m_raw);

		if (m_raw.type == AHA_CLSMEM_TYPE_FUNC)
		{
			m_params.resize(m_raw.function.CountOfParams);

			strm.read((char*)m_params.data(), sizeof(AhaType) * m_raw.function.CountOfParams);
			read += sizeof(AhaType) * m_raw.function.CountOfParams;

			m_opcode.resize(m_raw.function.SizeOfOpcode);

			strm.read((char*)m_opcode.data(), m_raw.function.SizeOfOpcode);
			read += m_raw.function.SizeOfOpcode;

			if (m_raw.function.SizeOfOpcode % 4 != 0)
			{
				size_t padding_sz = 4 - m_raw.function.SizeOfOpcode % 4;
				strm.read(padding, padding_sz);
				read += padding_sz;
			}
		}
	}

	void AhaClsMember::Validate(const AhaStrings& strings)
	{
		if (!ValidateAhaAccess(m_raw.access))
			throw BadModuleClsMemError();
		if (!ValidateAhaClsMemberType(m_raw.type))
			throw BadModuleClsMemError();
		if (!ValidateAhaClsMemberStorage(m_raw.storage))
			throw BadModuleClsMemError();
		if (!strings.ValidateString(m_raw.name))
			throw BadModuleClsMemError();

		if (!ValidateAhaType(m_raw.variable.vartype))
			throw BadModuleClsMemError();
		if (m_raw.variable._padding != 0)
			throw BadModuleClsMemError();

		if (m_raw.type == AHA_CLSMEM_TYPE_VAR)
		{
			if (m_raw.variable.vartype == AHA_TYPE_VOID)
				throw BadModuleClsMemError();
			
			uint8_t zero[sizeof(AhaVariable)] = { 0 };
			size_t z_sz = sizeof(AhaVariable) - AhaTypeGetSize(m_raw.type);
			if (z_sz != 0)
			{
				if (memcmp(&m_raw.variable.initial, zero, sizeof(AhaVariable)) != 0)
					throw BadModuleClsMemError();
			}
		}
	}

	const AhaClsMember_raw & AhaClsMember::GetRaw() const
	{
		return m_raw;
	}

	const std::vector<AhaType>& AhaClsMember::GetParams() const
	{
		return m_params;
	}

	const std::vector<aha_u8>& AhaClsMember::GetOpcode() const
	{
		return m_opcode;
	}
}
