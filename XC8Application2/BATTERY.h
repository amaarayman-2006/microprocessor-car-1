#ifndef BATTERY_H_
#define BATTERY_H_

#include "STD_Types.h"

/* =========================================================================
 *  BATTERY.h — موديل مراقبة جهد البطارية على ATmega328P
 *
 *  فكرة الدائرة:
 *  --------------
 *  بنوصّل + البطارية على Voltage Divider (مقسّم جهد) من مقاومتين متساويتين،
 *  والـ midpoint بنوصّله بـ ADC2 (PC2 = pin 25 على DIP-28).
 *  R1 = R2 = 10 kΩ → بنقسّم الجهد على 2 → القراءة بتفضل في حدود 0..5V
 *  حتى لو البطارية ممتلئة على 8.4V.
 *
 *      +V_batt  (جهد البطارية)
 *         |
 *       [R1]  (10 kΩ مربوطة على + البطارية)
 *         |
 *         +-------> PC2 / ADC2  (المدخل اللي بنقراه)
 *         |
 *       [R2]  (10 kΩ مربوطة على الأرض)
 *         |
 *        GND
 *
 *  المعادلة (حسابات صحيحة بدون float):
 *
 *      V_adc_mV  = ADC * VREF_mV / 1023
 *      V_batt_mV = V_adc_mV * (R1 + R2) / R2
 *
 *  الإعدادات تحت مظبوطة لبطارية 2S Li-ion (7.4V Nominal):
 *      ممتلئة ≈ 8.4V ، فاضية ≈ 6.0V ، حد التحذير = 6.5V
 *  لو بتشتغل ببطارية مختلفة، عدّل الـ MACROs اللي تحت.
 * ========================================================================= */

#define BATTERY_ADC_CHANNEL          2u              /* القناة: PC2 / ADC2  */
#define BATTERY_R1_OHMS              10000UL         /* مقاومة فوق المقسّم */
#define BATTERY_R2_OHMS              10000UL         /* مقاومة تحت المقسّم */
#define BATTERY_VREF_MV              5000UL          /* AVCC = 5V (بالمللي فولت) */
#define BATTERY_ADC_MAX              1023UL          /* أقصى قراءة لـ 10-bit ADC */

/* حدود التحذير (Hysteresis):
 *   - لو الجهد نزل تحت LOW_THRESHOLD       → ندوّر تنبيه
 *   - لو رجع فوق RECOVER_THRESHOLD          → نطفّي التنبيه
 *  الفرق بينهم بيمنع "الرفرفة" على حد العتبة.
 */
#define BATTERY_LOW_THRESHOLD_MV     6500UL          /* 6.5V → عتبة التحذير */
#define BATTERY_RECOVER_THRESHOLD_MV 6800UL          /* 6.8V → عتبة الرجوع   */

/* ------------- APIs ------------- */
void BATTERY_Init          (void);   /* مفيش حاجة نضبطها — PC2 مدخل تناظري بشكل افتراضي */
u16  BATTERY_ReadVoltage_mV(void);   /* بترجع جهد البطارية بالـ mV */
u8   BATTERY_IsLow         (void);   /* 1 = البطارية ضعيفة (مع Hysteresis) */

#endif /* BATTERY_H_ */
