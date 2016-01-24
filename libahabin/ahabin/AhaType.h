#pragma once

#include "ahabindef.h"

namespace ahabin
{
	// �������� ��쿡�� refer�� index
	//  -> 31bit�� 0
	// �⺻���� 31bit�� 1
	typedef aha_i32 AhaType;
	enum : AhaType
	{
		AHA_TYPE_VOID		= (AhaType)0x80000001,
		AHA_TYPE_BOOL		= (AhaType)0x80000002,
		AHA_TYPE_INT8		= (AhaType)0x80000003,
		AHA_TYPE_UINT8		= (AhaType)0x80000004,
		AHA_TYPE_INT16		= (AhaType)0x80000005,
		AHA_TYPE_UINT16		= (AhaType)0x80000006,
		AHA_TYPE_INT32		= (AhaType)0x80000007,
		AHA_TYPE_UINT32		= (AhaType)0x80000008,
		AHA_TYPE_INT64		= (AhaType)0x80000009,
		AHA_TYPE_UINT64		= (AhaType)0x8000000a,
		AHA_TYPE_FLOAT32	= (AhaType)0x8000000b,
		AHA_TYPE_FLOAT64	= (AhaType)0x8000000c,
		AHA_TYPE_INTPTR		= (AhaType)0x8000000d,
		AHA_TYPE_UINTPTR	= (AhaType)0x0000000e,
	};

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