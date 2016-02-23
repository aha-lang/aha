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
			m_locals.resize(m_raw.function.CountOfLocals);
			strm.read((char*)m_locals.data(), sizeof(AhaType) * m_raw.function.CountOfLocals);
			read += sizeof(AhaType) * m_raw.function.CountOfLocals;

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

	aha_u32 AhaClsMember::Write(std::ostream& strm)
	{
		aha_u32 written = 0;

		if (m_raw.type == AHA_CLSMEM_TYPE_FUNC)
		{
			m_raw.function.CountOfLocals = m_locals.size();
			m_raw.function.CountOfParams = m_params.size();
			m_raw.function.SizeOfOpcode = m_opcode.size();
		}
		else
		{
			m_raw.variable._padding = 0;
		}

		strm.write((const char*)&m_raw, sizeof(m_raw));
		written += sizeof(m_raw);

		if (m_raw.type == AHA_CLSMEM_TYPE_FUNC)
		{
			strm.write((const char*)m_locals.data(), sizeof(AhaType) * m_locals.size());
			written += sizeof(AhaType) * m_locals.size();

			strm.write((const char*)m_params.data(), sizeof(AhaType) * m_params.size());
			written += sizeof(AhaType) * m_params.size();

			strm.write((const char*)m_opcode.data(), m_opcode.size());
			written += m_opcode.size();

			if (m_opcode.size() % 4 != 0)
			{
				char padding[4] = { 0 };
				aha_u32 padding_sz = 4 - m_opcode.size() % 4;
				strm.write(padding, padding_sz);
				written += padding_sz;
			}
		}

		return written;
	}

	void AhaClsMember::Validate(const AhaStrings& strings) const
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

		if (m_raw.type == AHA_CLSMEM_TYPE_VAR)
		{
			if (m_raw.variable._padding != 0)
				throw BadModuleClsMemError();

			if (m_raw.variable.vartype == AHA_TYPE_VOID)
				throw BadModuleClsMemError();
			
			size_t v_sz = AhaTypeGetSize(m_raw.variable.vartype);
			if (v_sz != sizeof(AhaVariable))
			{
				uint8_t zero[sizeof(AhaVariable)] = { 0 };
				size_t z_sz = sizeof(AhaVariable) - v_sz;

				void* zr_mem = ((aha_u8*)&m_raw.variable.initial) + v_sz;

				if (memcmp(zr_mem, zero, z_sz) != 0)
					throw BadModuleClsMemError();
			}
		}
	}

	AhaClsMember_raw& AhaClsMember::GetRaw()
	{
		return m_raw;
	}
	const AhaClsMember_raw& AhaClsMember::GetRaw() const
	{
		return m_raw;
	}

	std::vector<AhaType>& AhaClsMember::GetLocals()
	{
		return m_locals;
	}
	const std::vector<AhaType>& AhaClsMember::GetLocals() const
	{
		return m_locals;
	}

	std::vector<AhaType>& AhaClsMember::GetParams()
	{
		return m_params;
	}
	const std::vector<AhaType>& AhaClsMember::GetParams() const
	{
		return m_params;
	}

	std::vector<aha_u8>& AhaClsMember::GetOpcode()
	{
		return m_opcode;
	}
	const std::vector<aha_u8>& AhaClsMember::GetOpcode() const
	{
		return m_opcode;
	}
}
