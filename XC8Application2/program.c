#include "types.h"
#include "private.h"
#include "interface.h"

/* =====================================================================
 *  program.c — تنفيذ درايڤر الـ ADC على ATmega328P
 *
 *  ملاحظات مهمّة عن الـ ADC على ATmega328P:
 *  --------------------------------------
 *   - في 6 قنوات Single-ended (ADC0..ADC5) على نسخة DIP-28،
 *     ADC6/ADC7 موجودين بس على نسخة TQFP.
 *   - REFS1:REFS0 = 00 → AREF خارجي
 *                  = 01 → AVCC مع كاباستور على AREF
 *                  = 11 → 1.1V داخلي
 *   - ساعة الـ ADC لازم تكون 50..200 kHz عشان دقّة 10-bit كاملة:
 *       Prescaler /128 → 16 MHz / 128 = 125 kHz   ✅ (Arduino UNO crystal)
 *       Prescaler /64  →  8 MHz /  64 = 125 kHz   ✅ (RC داخلي 8 MHz)
 * ===================================================================== */

void adc_init(voltage_ref vref, pre_Scalar Scalar)
{
    /* ----------- اختيار مرجع الجهد (REFS1:REFS0) ----------- */
    switch (vref)
    {
        case AREF:
            clearBIT(ADMUX, REFS1);
            clearBIT(ADMUX, REFS0);
            break;
        case AVCC:
            clearBIT(ADMUX, REFS1);
            setBIT  (ADMUX, REFS0);
            break;
        case INTERNAL:   /* مرجع داخلي 1.1V على ATmega328P */
            setBIT(ADMUX, REFS1);
            setBIT(ADMUX, REFS0);
            break;
    }

    /* ----------- تنسيق النتيجة: Right-adjusted (ADLAR = 0) ----------- *
     * يعني الـ 10 bits بتاعة النتيجة بتظهر:
     *   ADCH = bits 9..8 ، ADCL = bits 7..0
     * ده الأنسب لو هنقرا قيمة كاملة 10-bit.
     */
    clearBIT(ADMUX, ADLAR);

    /* ----------- اختيار الـ Prescaler (ADPS2:ADPS0) ----------- */
    /* بنمسحهم الأول قبل ما نضبطهم */
    clearBIT(ADCSRA, ADPS0);
    clearBIT(ADCSRA, ADPS1);
    clearBIT(ADCSRA, ADPS2);

    switch (Scalar)
    {
        case DIV_2:
            /* 000 أو 001 = /2 — الافتراضي بعد المسح */
            break;
        case DIV_4:
            setBIT(ADCSRA, ADPS1);
            break;
        case DIV_8:
            setBIT(ADCSRA, ADPS0);
            setBIT(ADCSRA, ADPS1);
            break;
        case DIV_16:
            setBIT(ADCSRA, ADPS2);
            break;
        case DIV_32:
            setBIT(ADCSRA, ADPS0);
            setBIT(ADCSRA, ADPS2);
            break;
        case DIV_64:
            setBIT(ADCSRA, ADPS1);
            setBIT(ADCSRA, ADPS2);
            break;
        case DIV_128:
            setBIT(ADCSRA, ADPS0);
            setBIT(ADCSRA, ADPS1);
            setBIT(ADCSRA, ADPS2);
            break;
    }

    /* ----------- تشغيل الـ ADC (ADEN = 1) ----------- */
    setBIT(ADCSRA, ADEN);
}

u16 analogRead(u8 channel)
{
    /* شبكة أمان: لو حد نسي يستدعي adc_init() — بنعمله auto-init */
    if (readBIT(ADCSRA, ADEN) == 0U)
    {
        adc_init(AVCC, DIV_128);
    }

    /* ----------- اختيار القناة (MUX3..MUX0) ----------- *
     * ADMUX:
     *   bits 7..6 = REFS1:REFS0 (مرجع الجهد)  ← نحافظ عليهم
     *   bit  5    = ADLAR                      ← نحافظ عليه
     *   bit  4    = محجوز (لازم 0)             ← قناع 0xF0 بيخلّيه 0 تلقائيًا
     *   bits 3..0 = MUX3..MUX0 (رقم القناة)    ← اللي بنحدّثه هنا
     */
    ADMUX = (u8)((ADMUX & 0xF0U) | (channel & 0x0FU));

    /* ----------- بدء التحويل (Start Conversion) ----------- */
    setBIT(ADCSRA, ADSC);

    /* ----------- انتظار انتهاء التحويل (ADSC بيتمسح أوتوماتيك) ----------- */
    while (readBIT(ADCSRA, ADSC))
    {
        /* لا حاجة — استنّ بس */
    }

    /* ----------- قراءة النتيجة ----------- *
     * مهم: لازم نقرا ADCL الأول، بعدها ADCH.
     * (قراءة ADCH هي اللي بتحرّر القفل الداخلي على الـ register).
     */
    u8 low  = ADCL;
    u8 high = ADCH;

    /* بما إن ADLAR = 0 → الـ 2 bits العلوية موجودين في ADCH[1:0] */
    return (u16)(((u16)(high & 0x03U) << 8) | low);
}
