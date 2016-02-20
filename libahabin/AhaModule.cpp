#include "ahabin/AhaModule.h"
#include "ahabin/exceptions.h"

namespace
{
	static constexpr aha::aha_u8 abf_mark[4] = { '\0', 'A', 'H', 'E' };
}

namespace aha
{
	void AhaModule::Read(std::istream& strm)
	{
		strm.exceptions(std::ios::badbit | std::ios::failbit);

		strm.read((char *)&m_Header, sizeof(m_Header));

		if (memcmp(&m_Header.mark, &abf_mark, sizeof(abf_mark)) != 0)
			throw BadModuleHeaderError();

		m_Strings.Read(m_Header.SizeOfStrings, strm);
		m_Refer.Read(m_Header.SizeOfRefer, strm);
		m_NativeRefer.Read(m_Header.SizeOfNativeRefer, strm);
		m_Body.Read(m_Header.SizeOfBody, strm);

		m_Refer.Validate(m_Strings);
		m_NativeRefer.Validate(m_Strings);
		m_Body.Validate(m_Strings);
	}

	AhaStrings& AhaModule::GetStrings()
	{
		return m_Strings;
	}

	AhaRefer& AhaModule::GetRefer()
	{
		return m_Refer;
	}

	AhaNativeRefer& AhaModule::GetNativeRefer()
	{
		return m_NativeRefer;
	}

	AhaBody& AhaModule::GetBody()
	{
		return m_Body;
	}

	aha_u32 AhaModule::GetModuleName() const
	{
		return m_Header.ModuleName;
	}
}
