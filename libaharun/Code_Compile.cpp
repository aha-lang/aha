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
					AHA_OPCODE_add_func,
					AHA_OPCODE_sub_func,
					AHA_OPCODE_mul_func,
					AHA_OPCODE_div_func,
					AHA_OPCODE_or_func,
					AHA_OPCODE_and_func,
					AHA_OPCODE_xor_func,
					AHA_OPCODE_shl_func,
					AHA_OPCODE_shr_func,
					AHA_OPCODE_not_func,
					AHA_OPCODE_dup_func,
					AHA_OPCODE_ldloc_func,
					AHA_OPCODE_stloc_func,
					AHA_OPCODE_ldvar_func,
					AHA_OPCODE_stvar_func,
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
					AHA_OPCODE_jmp_func,
					AHA_OPCODE_call_func,
					AHA_OPCODE_ret_func,
					AHA_OPCODE_je_func,
					AHA_OPCODE_jne_func,
					AHA_OPCODE_jl_func,
					AHA_OPCODE_jle_func,
					AHA_OPCODE_jg_func,
					AHA_OPCODE_jge_func,
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
					AHA_OPCODE_native_func,
					AHA_OPCODE_newobj_func,
					AHA_OPCODE_ldnull_func,
					AHA_OPCODE_ldpar_func,
					AHA_OPCODE_stpar_func,
					AHA_OPCODE_ldthis_func,
				};
				static_assert(sizeof(OpHandlerTable) / sizeof(OpHandlerTable[0]) == COUNT_AHA_OPCODE, "OpHandlerTable is wrong");

				;
			}

		private:
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

			static void AHA_OPCODE_ldvar_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_stvar_func(OpHandlerParam par)
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

			static void AHA_OPCODE_jmp_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_call_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ret_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_je_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jne_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jl_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jle_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jg_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_jge_func(OpHandlerParam par)
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

			static void AHA_OPCODE_native_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_newobj_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldnull_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldpar_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_stpar_func(OpHandlerParam par)
			{

			}

			static void AHA_OPCODE_ldthis_func(OpHandlerParam par)
			{

			}
		};
	}

	void Code::Compile()
	{
		detail::Compiler::DoCompile(this);
	}
}
