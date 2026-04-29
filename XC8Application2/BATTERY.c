#include "BATTERY.h"
#include "interface.h"   /* analogRead */

/* =====================================================================
 *  BATTERY.c — تنفيذ موديل البطارية
 *
 *  بنستخدم Hysteresis latch (مزلاج) عشان نتجنّب رفرفة الـ LED:
 *  - لما الجهد ينزل تحت العتبة المنخفضة → نقفل المزلاج (s_isLow = 1)
 *  - مفيش رجوع لـ 0 إلا لما الجهد يعدّي العتبة العليا (RECOVER)
 *  → ده بيعمل "Dead-band" حوالين العتبة فالـ LED مبيرفرفش لو الجهد
 *    عمّال يهتز حواليها.
 * ===================================================================== */

/* المزلاج: بيبدأ "البطارية كويسة" */
static u8 s_isLow = 0u;

void BATTERY_Init(void)
{
    /* بنّة PC2 (ADC2) هي بنّة تناظرية تلقائيًا.
     * القيم الافتراضية بعد الـ Reset:
     *   DDRC bit2 = 0  (مدخل)
     *   PORTC bit2 = 0  (مفيش Pull-up — وده اللي عايزينه للقراءة التناظرية)
     * يعني مفيش تهيئة GPIO مطلوبة هنا. بنرجّع المزلاج لصفر بس. */
    s_isLow = 0u;
}

u16 BATTERY_ReadVoltage_mV(void)
{
    /* بنقرا قيمة الـ ADC الخام (0..1023) من القناة المخصّصة للبطارية */
    u16 adc_value = analogRead((u8)BATTERY_ADC_CHANNEL);

    /* حسابات بـ Integer (مفيش Floating-point ↔ ده موفر CPU كتير على الـ AVR):
     *   V_adc_mV  = ADC * VREF_mV / 1023            (أقصاها 5000، تدخل في u16)
     *   V_batt_mV = V_adc_mV * (R1 + R2) / R2       (في حالتنا = ×2)
     *
     * أقصى قيمة وسطية ≈ 5000 * 20000 = 1e8 → آمنة جوّا u32. */
    u32 v_adc_mV  = ((u32)adc_value * (u32)BATTERY_VREF_MV) / (u32)BATTERY_ADC_MAX;
    u32 v_batt_mV = (v_adc_mV * (u32)(BATTERY_R1_OHMS + BATTERY_R2_OHMS))
                    / (u32)BATTERY_R2_OHMS;

    return (u16)v_batt_mV;
}

u8 BATTERY_IsLow(void)
{
    u16 v_mV = BATTERY_ReadVoltage_mV();

    if (s_isLow)
    {
        /* البطارية حاليًا "ضعيفة" → نطلب رجوع كامل عشان نعتبرها رجعت */
        if (v_mV >= BATTERY_RECOVER_THRESHOLD_MV)
        {
            s_isLow = 0u;   /* رجعت كويسة */
        }
    }
    else
    {
        /* البطارية حاليًا "كويسة" → نقفل المزلاج بس لو الجهد نزل تحت العتبة */
        if (v_mV <= BATTERY_LOW_THRESHOLD_MV)
        {
            s_isLow = 1u;   /* بقت ضعيفة */
        }
    }

    return s_isLow;
}
