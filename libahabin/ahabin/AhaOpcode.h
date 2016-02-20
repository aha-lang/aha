#pragma once

#include "ahabindef.h"

namespace aha
{
	enum AhaOpcode : aha_u8
	{
		AHA_OPCODE_add,
		AHA_OPCODE_sub,
		AHA_OPCODE_mul,
		AHA_OPCODE_div,
		AHA_OPCODE_or,
		AHA_OPCODE_and,
		AHA_OPCODE_xor,
		AHA_OPCODE_shl,
		AHA_OPCODE_shr,
		AHA_OPCODE_not,
		AHA_OPCODE_dup,
		AHA_OPCODE_ldloc,
		AHA_OPCODE_stloc,
		AHA_OPCODE_ldvar,
		AHA_OPCODE_stvar,
		AHA_OPCODE_ldstr,
		AHA_OPCODE_ldc_bool,
		AHA_OPCODE_ldc_byte,
		AHA_OPCODE_ldc_i8,
		AHA_OPCODE_ldc_i16,
		AHA_OPCODE_ldc_i32,
		AHA_OPCODE_ldc_i64,
		AHA_OPCODE_ldc_f32,
		AHA_OPCODE_ldc_f64,
		AHA_OPCODE_jmp,
		AHA_OPCODE_call,
		AHA_OPCODE_ret,
		AHA_OPCODE_locals,
		AHA_OPCODE_je,
		AHA_OPCODE_jne,
		AHA_OPCODE_jl,
		AHA_OPCODE_jle,
		AHA_OPCODE_jg,
		AHA_OPCODE_jge,
		AHA_OPCODE_conv_byte,
		AHA_OPCODE_conv_i8,
		AHA_OPCODE_conv_i16,
		AHA_OPCODE_conv_i32,
		AHA_OPCODE_conv_i64,
		AHA_OPCODE_conv_f32,
		AHA_OPCODE_conv_f64,
		AHA_OPCODE_conv_iptr,
		AHA_OPCODE_native,
		AHA_OPCODE_newobj,
		AHA_OPCODE_ldnull,
		AHA_OPCODE_ldpar,
		AHA_OPCODE_stpar,
		AHA_OPCODE_ldthis,
		COUNT_AHA_OPCODE
	};
	inline bool ValidateAhaOpcode(AhaOpcode i)
	{
		return (i < COUNT_AHA_OPCODE);
	}

	enum AhaNativeCallType : aha_u32
	{
		AHA_NATIVECALL_stdcall,
		AHA_NATIVECALL_cdecl,
		COUNT_AHA_NATIVECALL
	};
	inline bool ValidateAhaNativeCallType(AhaNativeCallType i)
	{
		return (i < COUNT_AHA_NATIVECALL);
	}
}
