#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/* =====================================================================
 *  STD_Types.h — تعريفات الأنواع الأساسية اللي بنستخدمها في كل المشروع
 *
 *  بنحب نختصر الأسماء الطويلة "unsigned char" إلخ بأسماء قصيرة وواضحة
 *  زي u8 و u16 — ده بيخلّي الكود يبقى أنضف وأسهل في القراءة.
 *
 *  u  = unsigned  (موجب فقط)
 *  s  = signed    (موجب وسالب)
 *  العدد بعد الحرف = عدد الـ bits
 * ===================================================================== */

typedef unsigned char       u8;    /* 8-bit  unsigned (0..255) */
typedef unsigned short int  u16;   /* 16-bit unsigned (0..65535) */
typedef unsigned int        u32;   /* 32-bit unsigned */
typedef unsigned long long  u64;   /* 64-bit unsigned */

typedef signed char         s8;    /* 8-bit  signed */
typedef signed short int    s16;   /* 16-bit signed */
typedef signed int          s32;   /* 32-bit signed */
typedef signed long long    s64;   /* 64-bit signed */

typedef float               f8;    /* عشري 32-bit (نادر استخدامه على AVR) */
typedef double              d64;   /* عشري 64-bit */

#endif /* STD_TYPES_H_ */
