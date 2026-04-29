#include "LED.h"

static u8 s_ledState = GPIO_LOW;

void LED_Init(void)
{

    GPIO_SetPinDirection(LED_LOW_BATT_PORT, LED_LOW_BATT_PIN, GPIO_OUTPUT);
    GPIO_WritePin       (LED_LOW_BATT_PORT, LED_LOW_BATT_PIN, GPIO_LOW);
    s_ledState = GPIO_LOW;
}

void LED_SetState(LED_State_t state)
{

    u8 level = (state == LED_STATE_ON) ? GPIO_HIGH : GPIO_LOW;
    GPIO_WritePin(LED_LOW_BATT_PORT, LED_LOW_BATT_PIN, level);
    s_ledState = level;
}

void LED_Toggle(void)
{

    s_ledState = (s_ledState == GPIO_HIGH) ? GPIO_LOW : GPIO_HIGH;
    GPIO_WritePin(LED_LOW_BATT_PORT, LED_LOW_BATT_PIN, s_ledState);
}

