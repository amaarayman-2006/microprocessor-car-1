#ifndef TYPES_H
#define TYPES_H

/* =====================================================================
 *  types.h — تعريفات الـ enums الخاصة بدرايڤر الـ ADC
 *
 *  الأنواع الأساسية (u8, u16, ..) جايه من STD_Types.h عشان مفيش تكرار.
 * ===================================================================== */
#include "STD_Types.h"

/* مرجع الجهد للـ ADC (REFS1:REFS0):
 * - AREF     : مصدر خارجي على بنّة AREF
 * - AVCC     : 5V الداخلي مع كاباستور على AREF (الأكثر استخدامًا)
 * - INTERNAL : مرجع داخلي 1.1V على ATmega328P (مفيد للجهود الصغيرة)
 */
typedef enum
{
    AREF      = 0,   /* AREF خارجي / Vref الداخلي مطفي */
    AVCC      = 1,   /* AVCC مع كاباستور على AREF       */
    INTERNAL  = 2    /* مرجع داخلي 1.1V على ATmega328P  */
} voltage_ref;

/* قنوات الـ ADC (ADC0..ADC7) */
typedef enum
{
    ADC_CH0 = 0,
    ADC_CH1,
    ADC_CH2,
    ADC_CH3,
    ADC_CH4,
    ADC_CH5,
    ADC_CH6,    /* متاحة بس على نسخة TQFP — مش موجودة على DIP-28 */
    ADC_CH7     /* نفس الكلام */
} adc_channel;

/* مقسّم تردد الـ ADC (ADPS2:ADPS0):
 *   F_ADC = F_CPU / Prescaler
 *   لازم F_ADC تكون بين 50..200 kHz عشان دقّة 10-bit كاملة.
 *   عند F_CPU = 16 MHz نختار DIV_128 → 125 kHz ✅
 */
typedef enum
{
    DIV_2 = 0,
    DIV_4,
    DIV_8,
    DIV_16,
    DIV_32,
    DIV_64,
    DIV_128
} pre_Scalar;

#endif /* TYPES_H */
