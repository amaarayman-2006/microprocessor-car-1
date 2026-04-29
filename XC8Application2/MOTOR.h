#ifndef MOTOR_H
#define MOTOR_H

#include "GPIO_interface.h"
#include "JOYSTICK.h"

/* =====================================================================
 *  MOTOR.h — واجهة موديل الموتورين (المحرّك الأيمن + المحرّك الأيسر)
 *
 *  بنوفّر مستويين من الـ APIs:
 *    1) Motor_Direction_t : أوامر "للعربية كلها" (قدام، ورا، يمين، شمال، وقوف).
 *    2) Motor_HalfDir_t   : تحكّم في كل موتور لوحده (يسار / يمين) ←
 *       ده اللي بيخلّينا نعمل الحركات القُطرية (Software-PWM على عجلة دون التانية).
 * ===================================================================== */

/* أوامر "العربية كلها" — منفصلة عن قِيَم الجويستيك عمدًا */
typedef enum
{
    MOTOR_STOP = 0,    /* وقوف كامل */
    MOTOR_FORWARD,     /* قدام */
    MOTOR_BACKWARD,    /* ورا  */
    MOTOR_LEFT,        /* لفّة شمال (Skid-steer) */
    MOTOR_RIGHT        /* لفّة يمين (Skid-steer) */
} Motor_Direction_t;

/* أوامر "موتور واحد بس" — بنستخدمها للتحكم في كل عجلة على حدة */
typedef enum
{
    MOTOR_HALF_STOP = 0,   /* العجلة دي واقفة */
    MOTOR_HALF_FORWARD,    /* العجلة دي بتلف لقدام */
    MOTOR_HALF_BACKWARD    /* العجلة دي بتلف لورا  */
} Motor_HalfDir_t;

/* ---------------- APIs ---------------- */

/* تهيئة بنّات الـ H-Bridge كـ OUTPUT + إيقاف الموتورين */
void MOTOR_Init                (void);

/* أمر "للعربية كلها" — بيعمل الحركة الأساسية بشكل كامل */
void MOTOR_Move                (Motor_Direction_t dir);

/* وقوف كامل للموتورين */
void MOTOR_Stop                (void);

/* تحكم في موتور واحد بس (يسار أو يمين) — ده مفتاح الحركات القُطرية */
void MOTOR_SetLeft             (Motor_HalfDir_t dir);
void MOTOR_SetRight            (Motor_HalfDir_t dir);

/* فنكشن الراحة: بتقرا الجويستيك وبتشغّل الموتورين على أساسها.
 * بنناديها كل لفّة في اللوب الرئيسي بتاع main(). */
void MOTOR_Update_FromJoystick (void);

#endif /* MOTOR_H */
