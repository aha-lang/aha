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
		AHA_TYPE_FLG_POINTER	= 0x40000000u,
		AHA_TYPE_FLG_PRIMITIVE	= 0x80000000u,
		AHA_TYPE_MASK_NONFLG	= 0x3fffffffu,

		AHA_TYPE_VOID			= 0,
		AHA_TYPE_BOOL			= 1,
		AHA_TYPE_INT8			= 2,
		AHA_TYPE_UINT8			= 3,
		AHA_TYPE_INT16			= 4,
		AHA_TYPE_UINT16			= 5,
		AHA_TYPE_INT32			= 6,
		AHA_TYPE_UINT32			= 7,
		AHA_TYPE_INT64			= 8,
		AHA_TYPE_UINT64			= 9,
		AHA_TYPE_FLOAT32		= 10,
		AHA_TYPE_FLOAT64		= 11,
		AHA_TYPE_INTPTR			= 12,
		AHA_TYPE_UINTPTR		= 13,
		AHA_TYPE_CHAR			= 14,
		COUNT_AHA_TYPE,
	};
	inline bool ValidateAhaType(AhaType type)
	{
		if (type & AHA_TYPE_FLG_PRIMITIVE)
		{
			// void*는 허용되지 않음
			if (type == (AHA_TYPE_FLG_PRIMITIVE | AHA_TYPE_FLG_POINTER | AHA_TYPE_VOID))
				return false;

			return (type & AHA_TYPE_MASK_NONFLG) < COUNT_AHA_TYPE;
		}
		else
		{
			return true;
		}
	}

	inline unsigned AhaTypeGetSize(AhaType type)
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
			sizeof(aha_char),	/* AHA_TYPE_CHAR */
		};

		if (type & AHA_TYPE_FLG_PRIMITIVE)
			return ar[type & AHA_TYPE_MASK_NONFLG];
		else
			return sizeof(void *);
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
		aha_char v_char;
		void *v_object;
	};
}
