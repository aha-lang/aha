#include "aharun/Code.h"

#include <ahabin/AhaOpcode.h>

#include "aharun/FunctionInfo.h"

namespace aha
{
	namespace detail
	{
		class Compiler
		{
		private:
			struct OpHandlerParam
			{
				Code *pCode;
			};
			typedef void (*OpHandler)(OpHandlerParam par);

		public:
			static void DoCompile(Code *pCode)
			{
				static OpHandler OpHandlerTable[] = {
					AHA_OPCODE_nop_func,
					AHA_OPCODE_pop_func,
					AHA_OPCODE_add_func,
					AHA_OPCODE_sub_func,
					AHA_OPCODE_mul_func,
					AHA_OPCODE_div_func,
					AHA_OPCODE_neg_func,
					AHA_OPCODE_or_func,
					AHA_OPCODE_and_func,
					AHA_OPCODE_xor_func,
					AHA_OPCODE_shl_func,
					AHA_OPCODE_shr_func,
					AHA_OPCODE_not_func,
					AHA_OPCODE_dup_func,
					AHA_OPCODE_ldloc_func,
					AHA_OPCODE_stloc_func,
					AHA_OPCODE_ldsfld_func,
					AHA_OPCODE_stsfld_func,
					AHA_OPCODE_ldarg_func,
					AHA_OPCODE_starg_func,
					AHA_OPCODE_ldptr_loc_func,
					AHA_OPCODE_ldptr_sfld_func,
					AHA_OPCODE_ldptr_arg_func,
					AHA_OPCODE_stptr_func,
					AHA_OPCODE_ldnull_func,
					AHA_OPCODE_ldstr_func,
					AHA_OPCODE_ldc_bool_func,
					AHA_OPCODE_ldc_i8_func,
					AHA_OPCODE_ldc_u8_func,
					AHA_OPCODE_ldc_i16_func,
					AHA_OPCODE_ldc_u16_func,
					AHA_OPCODE_ldc_i32_func,
					AHA_OPCODE_ldc_u32_func,
					AHA_OPCODE_ldc_i64_func,
					AHA_OPCODE_ldc_u64_func,
					AHA_OPCODE_ldc_f32_func,
					AHA_OPCODE_ldc_f64_func,
					AHA_OPCODE_conv_i8_func,
					AHA_OPCODE_conv_u8_func,
					AHA_OPCODE_conv_i16_func,
					AHA_OPCODE_conv_u16_func,
					AHA_OPCODE_conv_i32_func,
					AHA_OPCODE_conv_u32_func,
					AHA_OPCODE_conv_i64_func,
					AHA_OPCODE_conv_u64_func,
					AHA_OPCODE_conv_f32_func,
					AHA_OPCODE_conv_f64_func,
					AHA_OPCODE_conv_iptr_func,
					AHA_OPCODE_conv_uptr_func,
					AHA_OPCODE_jmp_func,
					AHA_OPCODE_jz_func,
					AHA_OPCODE_jnz_func,
					AHA_OPCODE_jnul_func,
					AHA_OPCODE_jobj_func,
					AHA_OPCODE_je_func,
					AHA_OPCODE_jne_func,
					AHA_OPCODE_jge_func,
					AHA_OPCODE_jg_func,
					AHA_OPCODE_jle_func,
					AHA_OPCODE_jl_func,
					AHA_OPCODE_cz_func,
					AHA_OPCODE_cnz_func,
					AHA_OPCODE_cnul_func,
					AHA_OPCODE_cobj_func,
					AHA_OPCODE_ce_func,
					AHA_OPCODE_cne_func,
					AHA_OPCODE_cge_func,
					AHA_OPCODE_cg_func,
					AHA_OPCODE_cle_func,
					AHA_OPCODE_call_func,
					AHA_OPCODE_callnative_func,
					AHA_OPCODE_ret_func,
					AHA_OPCODE_newobj_func,
				};
				static_assert(sizeof(OpHandlerTable) / sizeof(OpHandlerTable[0]) == COUNT_AHA_OPCODE, "OpHandlerTable is wrong");

				;
			}

		private:
			static void AHA_OPCODE_nop_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_pop_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_add_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_sub_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_mul_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_div_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_neg_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_or_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_and_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_xor_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_shl_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_shr_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_not_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_dup_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldloc_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_stloc_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldsfld_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_stsfld_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldarg_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_starg_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldptr_loc_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldptr_sfld_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldptr_arg_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_stptr_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldnull_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldstr_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_bool_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_i8_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_u8_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_i16_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_u16_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_i32_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_u32_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_i64_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_u64_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_f32_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldc_f64_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_i8_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_u8_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_i16_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_u16_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_i32_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_u32_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_i64_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_u64_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_f32_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_f64_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_iptr_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_conv_uptr_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jmp_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jz_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jnz_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jnul_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jobj_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_je_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jne_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jge_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jg_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jle_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jl_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_cz_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_cnz_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_cnul_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_cobj_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ce_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_cne_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_cge_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_cg_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_cle_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_call_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_callnative_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ret_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_newobj_func(OpHandlerParam par)
			{

			}
		};
	}

	void Code::Compile()
	{
		detail::Compiler::DoCompile(this);
	}
}
