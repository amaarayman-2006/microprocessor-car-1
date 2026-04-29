#ifndef PRIVATE_H
#define PRIVATE_H

#include "types.h"

/* =====================================================================
 *  private.h — تعريفات داخلية لدرايڤر الـ ADC على ATmega328P
 *
 *  بنحدّد هنا:
 *    1) عناوين الرجسترات الخاصة بالـ ADC (Memory-mapped IO)
 *    2) أرقام البيتس (Bit positions) داخل كل register
 *    3) MACROs مساعدة للعمليات على البيتس
 *
 *  العناوين دي من الـ Datasheet (Data-space view):
 *      ADCL   = 0x78   →  بيت أدنى من نتيجة التحويل
 *      ADCH   = 0x79   →  بيت أعلى من نتيجة التحويل
 *      ADCSRA = 0x7A   →  Control & Status A (تشغيل، prescaler، start...)
 *      ADCSRB = 0x7B   →  Control B (مش مستخدم في الدرايڤر ده)
 *      ADMUX  = 0x7C   →  Multiplexer (اختيار القناة + المرجع + ADLAR)
 *      DIDR0  = 0x7E   →  Disable digital input على بنّات الـ ADC (اختياري)
 *
 *  العناوين دي مظبوطة لـ ATmega328P (شريحة الـ Arduino UNO).
 * ===================================================================== */

#define ADCL    (*((volatile u8*)0x78))   /* نتيجة التحويل — البايت الأدنى */
#define ADCH    (*((volatile u8*)0x79))   /* نتيجة التحويل — البايت الأعلى */
#define ADCSRA  (*((volatile u8*)0x7A))   /* Control & Status A */
#define ADMUX   (*((volatile u8*)0x7C))   /* Multiplexer */

/* بيتس داخل ADCSRA */
#define ADPS0   0   /* Prescaler bit 0 */
#define ADPS1   1   /* Prescaler bit 1 */
#define ADPS2   2   /* Prescaler bit 2 */
#define ADIE    3   /* Interrupt Enable */
#define ADIF    4   /* Interrupt Flag   */
#define ADATE   5   /* Auto Trigger Enable */
#define ADSC    6   /* Start Conversion */
#define ADEN    7   /* ADC Enable       */

/* بيتس داخل ADMUX
 * (على ATmega328P في بس MUX3..MUX0 ، البِت 4 محجوز ولازم يفضل 0) */
#define MUX0    0
#define MUX1    1
#define MUX2    2
#define MUX3    3
#define ADLAR   5   /* Left Adjust Result (0 = right-adjusted) */
#define REFS0   6   /* Voltage Reference Select 0 */
#define REFS1   7   /* Voltage Reference Select 1 */

/* MACROs مساعدة (داخلية بس لدرايڤر الـ ADC) */
#define setBIT(reg, pin)    ((reg) |=  (1U << (pin)))   /* خلّي البِت = 1 */
#define clearBIT(reg, pin)  ((reg) &= ~(1U << (pin)))   /* خلّي البِت = 0 */
#define readBIT(reg, pin)   (((reg) & (1U << (pin))) != 0U)  /* اقرا البِت */

#endif /* PRIVATE_H */
