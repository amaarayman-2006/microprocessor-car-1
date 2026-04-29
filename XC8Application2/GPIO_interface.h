#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

#include "STD_Types.h"

/* =====================================================================
 *  GPIO_interface.h — الواجهة العامة لطبقة الـ GPIO (MCAL)
 *
 *  بنعرّف هنا كل الـ MACROs والـ APIs اللي طبقة الـ HAL والـ APP
 *  بيستخدموها للتحكم في البنّات الرقمية.
 * ===================================================================== */

/* اتجاه البنّة (Direction) */
#define GPIO_INPUT      0   /* مدخل */
#define GPIO_OUTPUT     1   /* مخرج */

/* مستوى الإشارة (Logic level) */
#define GPIO_LOW        0   /* 0 فولت */
#define GPIO_HIGH       1   /* 5 فولت */

/* معرّفات البورتس:
 * --------------
 * ATmega328P عنده 3 بورتس فقط: PORTB, PORTC, PORTD
 * (مفيش PORTA زي ATmega32 — فيه 6 بنّات بس على PORTC = ADC0..ADC5).
 */
#define GPIO_PORTB      0
#define GPIO_PORTC      1
#define GPIO_PORTD      2

/* أرقام البنّات (Pin numbers) من 0 لـ 7 */
#define GPIO_PIN0       0
#define GPIO_PIN1       1
#define GPIO_PIN2       2
#define GPIO_PIN3       3
#define GPIO_PIN4       4
#define GPIO_PIN5       5
#define GPIO_PIN6       6
#define GPIO_PIN7       7

/* ------------- APIs ------------- */
void GPIO_SetPinDirection(u8 Port, u8 Pin, u8 Direction);  /* تحديد اتجاه البنّة */
void GPIO_WritePin       (u8 Port, u8 Pin, u8 Value);      /* كتابة 0 / 1 على البنّة */
u8   GPIO_ReadPin        (u8 Port, u8 Pin);                /* قراءة حالة البنّة */

#endif /* GPIO_INTERFACE_H_ */
