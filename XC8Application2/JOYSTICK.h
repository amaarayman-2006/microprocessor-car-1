#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "STD_Types.h"
#include "interface.h"          /* ADC API: analogRead */
#include "GPIO_interface.h"     /* GPIO: لقراءة زر الجويستيك */

/* =====================================================================
 * توصيلات الجويستيك على Arduino UNO (ATmega328P) — حسب توصيلتك الفعلية
 *
 *   VRX (محور X / يمين-شمال) ──► A0  (PC0 / ADC0)
 *   VRY (محور Y / قدام-ورا)  ──► A1  (PC1 / ADC1)
 *   SW  (الضغطة)             ──► D2  (PD2 / INT0)  — active-LOW pull-up
 *   VCC ──► +5V              GND ──► GND
 * ===================================================================== */

/* قنوات الـ ADC (نفس ترقيم Arduino A0..A5) */
#define JOYSTICK_X_CH            0u    /* A0 = PC0 = ADC0 — محور X (يمين/شمال) */
#define JOYSTICK_Y_CH            1u    /* A1 = PC1 = ADC1 — محور Y (قدام/ورا) */

/* زر الجويستيك */
#define JOYSTICK_PB_PORT         GPIO_PORTD
#define JOYSTICK_PB_PIN          GPIO_PIN2     /* D2 = PD2 */

/* مدى الـ ADC (10-bit) ومنطقة الراحة (Dead-zone) */
#define JOYSTICK_ADC_MAX         1023u
#define JOYSTICK_CENTER          (JOYSTICK_ADC_MAX / 2u)        /* = 511 (مركز الجويستيك) */
#define JOYSTICK_DEAD_ZONE       150u                            /* تسامح ±150 حول المركز */
#define JOYSTICK_HIGH_THRESH     (JOYSTICK_CENTER + JOYSTICK_DEAD_ZONE)  /* 661 */
#define JOYSTICK_LOW_THRESH      (JOYSTICK_CENTER - JOYSTICK_DEAD_ZONE)  /* 361 */

/* القراءة الخام من المحورين (0..1023) */
typedef struct
{
    u16 x;   /* 0 = شمال، 1023 = يمين     */
    u16 y;   /* 0 = ورا،  1023 = قدام     */
} Joystick_RawData_t;

/* =====================================================================
 * كل الاتجاهات الممكنة من الجويستيك
 * (الـ 4 الأساسية + الـ 4 القُطرية + الوقوف)
 * ===================================================================== */
typedef enum
{
    JOYSTICK_DIR_STOP = 0,        /* الجويستيك في الراحة (داخل الـ dead-zone) */
    JOYSTICK_DIR_FORWARD,         /* قدام */
    JOYSTICK_DIR_BACKWARD,        /* ورا */
    JOYSTICK_DIR_LEFT,            /* شمال خالص */
    JOYSTICK_DIR_RIGHT,           /* يمين خالص */
    JOYSTICK_DIR_FORWARD_LEFT,    /* قدام + شمال (قُطري) */
    JOYSTICK_DIR_FORWARD_RIGHT,   /* قدام + يمين (قُطري) */
    JOYSTICK_DIR_BACKWARD_LEFT,   /* ورا + شمال (قُطري)  */
    JOYSTICK_DIR_BACKWARD_RIGHT   /* ورا + يمين (قُطري)  */
} Joystick_Direction_t;

/* APIs */
void                  JOYSTICK_INIT        (void);                         /* تهيئة الـ ADC + زر */
void                  JOYSTICK_READ_RAW    (Joystick_RawData_t *pData);    /* قراءة خام للمحورين */
Joystick_Direction_t  JOYSTICK_GET_DIR     (void);                         /* استنتاج الاتجاه */
u8                    JOYSTICK_IsBtnPressed(void);                         /* 1 = مضغوط، 0 = لا */

#endif /* JOYSTICK_H_ */
