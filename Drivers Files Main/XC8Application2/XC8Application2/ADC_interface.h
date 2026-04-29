#ifndef INTERFACE_H
#define INTERFACE_H

#include "types.h"

/* =====================================================================
 *  interface.h — الواجهة العامة لدرايڤر الـ ADC على ATmega328P
 *
 *  الـ ADC = Analog-to-Digital Converter
 *  وظيفته: يحوّل جهد كهربي تماثلي (0..5V) لقيمة رقمية (0..1023).
 * ===================================================================== */

/* تهيئة الـ ADC مرة واحدة عند بداية البرنامج:
 *   - vref   : مرجع الجهد (AREF / AVCC / INTERNAL)
 *   - Scalar : Prescaler (مهم نختاره عشان F_ADC تكون 50..200 kHz)
 */
void adc_init(voltage_ref vref, pre_Scalar Scalar);

/* قراءة 10-bit (0..1023) من قناة معيّنة:
 *   - channel: من 0 لـ 7 (Single-ended)
 *   - بيرجّع القراءة كـ u16
 */
u16  analogRead(u8 channel);

#endif /* INTERFACE_H */
