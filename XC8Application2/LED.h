#ifndef LED_H_
#define LED_H_

#include "STD_Types.h"
#include "GPIO_interface.h"

/* =====================================================================
 *  LED.h — موديل الـ LED الأحمر اللي بيدّينا تنبيه "البطارية ضعيفة"
 *
 *  اختيار البنّة (Pin) على ATmega328P:
 *  -----------------------------------
 *  بنستخدم PB0 (pin 14 على DIP-28 = D8 على الـ Arduino UNO).
 *  السبب: بنّات PORTC محجوزة للـ 3 قنوات ADC اللي شغّالين عندنا
 *         (Joystick X / Joystick Y / Battery sense)، فـ PB0 أنسب
 *         بنّة فاضية لـ LED مخرج (Output) واحد.
 *
 *  التوصيلة:
 *      PB0 (D8)  ──[ مقاومة 330Ω ]──▶|◀── LED ──── GND
 *                                  Anode  Cathode
 *
 *  منطق التشغيل: Active-HIGH → كتابة 1 على البنّة بتشغل الـ LED.
 * ===================================================================== */
#define LED_LOW_BATT_PORT   GPIO_PORTB
#define LED_LOW_BATT_PIN    GPIO_PIN0   /* D8 = PB0 */

/* حالات الـ LED */
typedef enum
{
    LED_STATE_OFF = 0,   /* مطفي */
    LED_STATE_ON  = 1    /* مولّع */
} LED_State_t;

/* ------------- APIs ------------- */
void LED_Init    (void);                /* تجهيز البنّة كـ OUTPUT + إطفاء الـ LED */
void LED_SetState(LED_State_t state);   /* تشغيل/إطفاء الـ LED */
void LED_Toggle  (void);                /* عكس حالة الـ LED */

#endif /* LED_H_ */
