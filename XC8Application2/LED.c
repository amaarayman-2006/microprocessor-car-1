#include "LED.h"

/* =====================================================================
 *  LED.c — تنفيذ موديل الـ LED
 *  بنحتفظ بالحالة الحالية في متغير static عشان نقدر نعمل Toggle بسهولة.
 * ===================================================================== */

/* الحالة الداخلية (مش ظاهرة برّا الفايل) — تبدأ مطفية */
static u8 s_ledState = GPIO_LOW;

void LED_Init(void)
{
    /* نخلّي البنّة Output ونبدأ بالـ LED مطفي (Safe state) */
    GPIO_SetPinDirection(LED_LOW_BATT_PORT, LED_LOW_BATT_PIN, GPIO_OUTPUT);
    GPIO_WritePin       (LED_LOW_BATT_PORT, LED_LOW_BATT_PIN, GPIO_LOW);
    s_ledState = GPIO_LOW;
}

void LED_SetState(LED_State_t state)
{
    /* لو الحالة المطلوبة ON → نكتب HIGH، غير كده LOW */
    u8 level = (state == LED_STATE_ON) ? GPIO_HIGH : GPIO_LOW;
    GPIO_WritePin(LED_LOW_BATT_PORT, LED_LOW_BATT_PIN, level);
    s_ledState = level;
}

void LED_Toggle(void)
{
    /* بنعكس الحالة الداخلية ونكتبها على البنّة */
    s_ledState = (s_ledState == GPIO_HIGH) ? GPIO_LOW : GPIO_HIGH;
    GPIO_WritePin(LED_LOW_BATT_PORT, LED_LOW_BATT_PIN, s_ledState);
}
