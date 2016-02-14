#pragma once

#include "ahabindef.h"
#include "AhaAccess.h"
#include "AhaType.h"

namespace aha
{
	enum AhaClsMemberType
	{
		AHA_CLSMEM_TYPE_FUNC,
		AHA_CLSMEM_TYPE_VAR,
		COUNT_AHA_CLSMEM
	};

	enum AhaClsMemberStorage
	{
		AHA_CLSMEM_STORAGE_CLASSOF,
		AHA_CLSMEM_STORAGE_INSTOF,
		COUNT_AHA_CLSMEM_STORAGE
	};

	struct AhaClsMember_raw
	{
		AhaAccess access;
		AhaClsMemberType type;
		AhaClsMemberStorage storage;
		aha_i32 name;

		union
		{
			struct
			{
				AhaType vartype;
				aha_i32 _padding;
				AhaVariable initial;
			} variable;

			struct
			{
				AhaType rettype;
				aha_i32 _padding;
				aha_i32 CountOfParams;
				aha_i32 SizeOfOpcode;
				// AhaType params[CountOfParams];
				// aha_byte opcode[SizeOfOpcode];
			} function;
		};
	};

	class AhaClsMember
	{
	private:
		AhaClsMember_raw m_raw;
		std::vector<AhaType> m_params;
		std::vector<aha_u8> m_opcode;

	public:
		void Read(std::istream& strm, size_t& read);

		const AhaClsMember_raw& GetRaw() const;
		const std::vector<AhaType>& GetParams() const;
		const std::vector<aha_u8>& GetOpcode() const;
	};
}
