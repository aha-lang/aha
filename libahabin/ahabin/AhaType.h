#pragma once

#include "ahabindef.h"

namespace aha
{
	// 참조형의 경우에는 refer의 index
	//  -> 31bit가 0
	// 기본형은 31bit가 1
	typedef aha_u32 AhaType;
	enum : AhaType
	{
		AHA_TYPE_VOID		= 0x80000000u,
		AHA_TYPE_BOOL		= 0x80000001u,
		AHA_TYPE_INT8		= 0x80000002u,
		AHA_TYPE_UINT8		= 0x80000003u,
		AHA_TYPE_INT16		= 0x80000004u,
		AHA_TYPE_UINT16		= 0x80000005u,
		AHA_TYPE_INT32		= 0x80000006u,
		AHA_TYPE_UINT32		= 0x80000007u,
		AHA_TYPE_INT64		= 0x80000008u,
		AHA_TYPE_UINT64		= 0x80000009u,
		AHA_TYPE_FLOAT32	= 0x8000000au,
		AHA_TYPE_FLOAT64	= 0x8000000bu,
		AHA_TYPE_INTPTR		= 0x8000000cu,
		AHA_TYPE_UINTPTR	= 0x0000000du,
		MAX_AHA_TYPE_PRIMITIVE = AHA_TYPE_UINTPTR,
	};
	inline bool ValidateAhaType(AhaType i)
	{
		return (!(i & 0x80000000) || i <= MAX_AHA_TYPE_PRIMITIVE);
	}

	inline size_t AhaTypeGetSize(AhaType type)
	{
		static const size_t ar[] = {
			0,					/* AHA_TYPE_VOID */
			sizeof(aha_bool),	/* AHA_TYPE_BOOL */
			sizeof(aha_i8),		/* AHA_TYPE_INT8 */
			sizeof(aha_u8),		/* AHA_TYPE_UINT8 */
			sizeof(aha_i16),	/* AHA_TYPE_INT16 */
			sizeof(aha_u16),	/* AHA_TYPE_UINT16 */
			sizeof(aha_i32),	/* AHA_TYPE_INT32 */
			sizeof(aha_u32),	/* AHA_TYPE_UINT32 */
			sizeof(aha_i64),	/* AHA_TYPE_INT64 */
			sizeof(aha_u64),	/* AHA_TYPE_UINT64 */
			sizeof(aha_f32),	/* AHA_TYPE_FLOAT32 */
			sizeof(aha_f64),	/* AHA_TYPE_FLOAT64 */
			sizeof(aha_iptr),	/* AHA_TYPE_INTPTR */
			sizeof(aha_uptr),	/* AHA_TYPE_UINTPTR */
		};
		return (type & 0x80000000) ? ar[type & 0x7fffffff] : sizeof(void *);
	}

	union AhaVariable
	{
		aha_bool v_bool;
		aha_i8 v_int8;
		aha_u8 v_uint8;
		aha_i16 v_int16;
		aha_u16 v_uint16;
		aha_i32 v_int32;
		aha_u32 v_uint32;
		aha_i64 v_int64;
		aha_u64 v_uint64;
		aha_f32 v_float32;
		aha_f64 v_float64;
		aha_iptr v_intptr;
		aha_uptr v_uintptr;
		void *v_object;
	};
}
