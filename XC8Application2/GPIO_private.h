#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

#include "STD_Types.h"

/* =====================================================================
 *  GPIO_private.h — عناوين الـ Registers الخاصة بالـ GPIO على ATmega328P
 *
 *  الـ ATmega328P (نفس شريحة الـ Arduino UNO) عنده 3 بورتس بس:
 *      PORTB, PORTC, PORTD   (مفيش PORTA — ده موجود في ATmega32 فقط)
 *
 *  لكل بورت 3 رجسترات:
 *  --------------------
 *      PINx   →  لقراءة حالة البنّات (Input)
 *      DDRx   →  لتحديد اتجاه البنّات (0 = Input ، 1 = Output)
 *      PORTx  →  لكتابة القيمة (Output)، أو لتفعيل الـ Pull-up (Input)
 *
 *  العناوين دي من الـ Datasheet (Data-space view):
 *      PINB  = 0x23 , DDRB  = 0x24 , PORTB = 0x25
 *      PINC  = 0x26 , DDRC  = 0x27 , PORTC = 0x28
 *      PIND  = 0x29 , DDRD  = 0x2A , PORTD = 0x2B
 *
 *  بنعمل لها typedef بـ volatile عشان الـ compiler ميعملش لها optimization
 *  ويضمن إنها تتقرا/تتكتب فعليًا من العتاد.
 * ===================================================================== */

#define PINB    (*((volatile u8*)0x23))   /* قراءة PORTB */
#define DDRB    (*((volatile u8*)0x24))   /* اتجاه PORTB */
#define PORTB   (*((volatile u8*)0x25))   /* كتابة PORTB */

#define PINC    (*((volatile u8*)0x26))   /* قراءة PORTC */
#define DDRC    (*((volatile u8*)0x27))   /* اتجاه PORTC */
#define PORTC   (*((volatile u8*)0x28))   /* كتابة PORTC */

#define PIND    (*((volatile u8*)0x29))   /* قراءة PORTD */
#define DDRD    (*((volatile u8*)0x2A))   /* اتجاه PORTD */
#define PORTD   (*((volatile u8*)0x2B))   /* كتابة PORTD */

#endif /* GPIO_PRIVATE_H_ */
