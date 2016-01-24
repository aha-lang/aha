#pragma once

#include "ahabindef.h"
#include "Result.h"
#include "AhaAccess.h"
#include "AhaType.h"
#include "ArrayList.h"

namespace ahabin
{
	class ReadStream;

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
		ArrayList<AhaType> m_params;
		ArrayList<aha_u8> m_opcode;

	public:
		Result Read(ReadStream& strm, size_t& read);

		const AhaClsMember_raw& GetRaw() const;
		const ArrayList<AhaType>& GetParams() const;
		const ArrayList<aha_u8>& GetOpcode() const;
	};
}
