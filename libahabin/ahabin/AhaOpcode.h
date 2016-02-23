#pragma once

#include "ahabindef.h"

namespace aha
{
	enum AhaOpcode : aha_u8
	{
		AHA_OPCODE_nop,
		AHA_OPCODE_pop,
		AHA_OPCODE_add,
		AHA_OPCODE_sub,
		AHA_OPCODE_mul,
		AHA_OPCODE_div,
		AHA_OPCODE_neg,
		AHA_OPCODE_or,
		AHA_OPCODE_and,
		AHA_OPCODE_xor,
		AHA_OPCODE_shl,
		AHA_OPCODE_shr,
		AHA_OPCODE_not,
		AHA_OPCODE_dup,
		AHA_OPCODE_ldloc,
		AHA_OPCODE_stloc,
		AHA_OPCODE_ldsfld,
		AHA_OPCODE_stsfld,
		AHA_OPCODE_ldarg,
		AHA_OPCODE_starg,
		AHA_OPCODE_ldptr_loc,
		AHA_OPCODE_ldptr_sfld,
		AHA_OPCODE_ldptr_arg,
		AHA_OPCODE_stptr,
		AHA_OPCODE_ldnull,
		AHA_OPCODE_ldstr,
		AHA_OPCODE_ldc_bool,
		AHA_OPCODE_ldc_i8,
		AHA_OPCODE_ldc_u8,
		AHA_OPCODE_ldc_i16,
		AHA_OPCODE_ldc_u16,
		AHA_OPCODE_ldc_i32,
		AHA_OPCODE_ldc_u32,
		AHA_OPCODE_ldc_i64,
		AHA_OPCODE_ldc_u64,
		AHA_OPCODE_ldc_f32,
		AHA_OPCODE_ldc_f64,
		AHA_OPCODE_conv_i8,
		AHA_OPCODE_conv_u8,
		AHA_OPCODE_conv_i16,
		AHA_OPCODE_conv_u16,
		AHA_OPCODE_conv_i32,
		AHA_OPCODE_conv_u32,
		AHA_OPCODE_conv_i64,
		AHA_OPCODE_conv_u64,
		AHA_OPCODE_conv_f32,
		AHA_OPCODE_conv_f64,
		AHA_OPCODE_conv_iptr,
		AHA_OPCODE_conv_uptr,
		AHA_OPCODE_jmp,
		AHA_OPCODE_jz,
		AHA_OPCODE_jnz,
		AHA_OPCODE_jnul,
		AHA_OPCODE_jobj,
		AHA_OPCODE_je,
		AHA_OPCODE_jne,
		AHA_OPCODE_jge,
		AHA_OPCODE_jg,
		AHA_OPCODE_jle,
		AHA_OPCODE_jl,
		AHA_OPCODE_cz,
		AHA_OPCODE_cnz,
		AHA_OPCODE_cnul,
		AHA_OPCODE_cobj,
		AHA_OPCODE_ce,
		AHA_OPCODE_cne,
		AHA_OPCODE_cge,
		AHA_OPCODE_cg,
		AHA_OPCODE_cle,
		AHA_OPCODE_call,
		AHA_OPCODE_callnative,
		AHA_OPCODE_ret,
		AHA_OPCODE_newobj,
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
