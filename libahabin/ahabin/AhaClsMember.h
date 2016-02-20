#pragma once

#include "ahabindef.h"
#include "AhaAccess.h"
#include "AhaType.h"

namespace aha
{
	enum AhaClsMemberType : aha_u32
	{
		AHA_CLSMEM_TYPE_FUNC,
		AHA_CLSMEM_TYPE_VAR,
		COUNT_AHA_CLSMEM
	};
	inline bool ValidateAhaClsMemberType(AhaClsMemberType i)
	{
		return (i < COUNT_AHA_CLSMEM);
	}

	enum AhaClsMemberStorage : aha_u32
	{
		AHA_CLSMEM_STORAGE_CLASSOF,
		AHA_CLSMEM_STORAGE_INSTOF,
		COUNT_AHA_CLSMEM_STORAGE
	};
	inline bool ValidateAhaClsMemberStorage(AhaClsMemberStorage i)
	{
		return i < COUNT_AHA_CLSMEM_STORAGE;
	}

	struct AhaClsMember_raw
	{
		AhaAccess access;
		AhaClsMemberType type;
		AhaClsMemberStorage storage;
		aha_u32 name;

		union
		{
			struct
			{
				AhaType vartype;
				aha_u32 _padding;
				AhaVariable initial;
			} variable;

			struct
			{
				AhaType rettype;
				aha_u32 _padding;
				aha_u32 CountOfParams;
				aha_u32 SizeOfOpcode;
				// AhaType params[CountOfParams];
				// aha_byte opcode[SizeOfOpcode];
			} function;
		};
	};

	class AhaStrings;

	class AhaClsMember
	{
	private:
		AhaClsMember_raw m_raw;
		std::vector<AhaType> m_params;
		std::vector<aha_u8> m_opcode;

	public:
		void Read(std::istream& strm, size_t& read);
		void Validate(const AhaStrings& strings) const;

		AhaClsMember_raw& GetRaw();
		const AhaClsMember_raw& GetRaw() const;

		std::vector<AhaType>& GetParams();
		const std::vector<AhaType>& GetParams() const;

		std::vector<aha_u8>& GetOpcode();
		const std::vector<aha_u8>& GetOpcode() const;
	};
}
