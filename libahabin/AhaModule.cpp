#include "ahabin\AhaModule.h"
#include "ahabin/AhaModule.h"
#include "ahabin/exceptions.h"

namespace
{
	static constexpr aha::aha_u8 abf_mark[4] = { '\0', 'A', 'H', 'E' };
}

namespace aha
{
	void AhaModule::Read(std::istream& strm, bool bValidate /* = true */)
	{
		strm.exceptions(std::ios::badbit | std::ios::failbit);

		strm.read((char *)&m_Header, sizeof(m_Header));

		if (memcmp(m_Header.mark, abf_mark, sizeof(abf_mark)) != 0)
			throw BadModuleHeaderError();

		m_Strings.Read(m_Header.SizeOfStrings, strm);
		m_Refer.Read(m_Header.SizeOfRefer, strm);
		m_NativeRefer.Read(m_Header.SizeOfNativeRefer, strm);
		m_Body.Read(m_Header.SizeOfBody, strm);

		if (bValidate)
			Validate();
	}

	void AhaModule::Write(std::ostream& strm, bool bValidate /* = true */)
	{
		if (bValidate)
			Validate();

		strm.exceptions(std::ios::badbit | std::ios::failbit);

		strm.write((const char*)&m_Header, sizeof(m_Header));

		memcpy(m_Header.mark, abf_mark, sizeof(abf_mark));
		m_Header.SizeOfStrings = m_Strings.Write(strm);
		m_Header.SizeOfRefer = m_Refer.Write(strm);
		m_Header.SizeOfNativeRefer = m_NativeRefer.Write(strm);
		m_Header.SizeOfBody = m_Body.Write(strm);

		int sz = sizeof(m_Header)
			+ m_Header.SizeOfStrings + m_Header.SizeOfRefer
			+ m_Header.SizeOfNativeRefer + m_Header.SizeOfBody;
		strm.seekp(-sz, std::ios::cur);
		strm.write((const char*)&m_Header, sizeof(m_Header));
	}

	void AhaModule::Validate() const
	{
		m_Refer.Validate(m_Strings);
		m_NativeRefer.Validate(m_Strings);
		m_Body.Validate(m_Strings);
	}

	AhaModuleHeader & AhaModule::GetHeader()
	{
		return m_Header;
	}
	const AhaModuleHeader & AhaModule::GetHeader() const
	{
		return m_Header;
	}

	AhaStrings& AhaModule::GetStrings()
	{
		return m_Strings;
	}
	const AhaStrings& AhaModule::GetStrings() const
	{
		return m_Strings;
	}

	AhaRefer& AhaModule::GetRefer()
	{
		return m_Refer;
	}
	const AhaRefer& AhaModule::GetRefer() const
	{
		return m_Refer;
	}

	AhaNativeRefer& AhaModule::GetNativeRefer()
	{
		return m_NativeRefer;
	}
	const AhaNativeRefer& AhaModule::GetNativeRefer() const
	{
		return m_NativeRefer;
	}

	AhaBody& AhaModule::GetBody()
	{
		return m_Body;
	}
	const AhaBody& AhaModule::GetBody() const
	{
		return m_Body;
	}

	aha_u32 AhaModule::GetModuleName() const
	{
		return m_Header.ModuleName;
	}
}
