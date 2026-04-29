#include "JOYSTICK.h"

void JOYSTICK_INIT(void)
{

    adc_init(AVCC, DIV_128);

    GPIO_SetPinDirection(JOYSTICK_PB_PORT, JOYSTICK_PB_PIN, GPIO_INPUT);
    GPIO_WritePin       (JOYSTICK_PB_PORT, JOYSTICK_PB_PIN, GPIO_HIGH);
}

void JOYSTICK_READ_RAW(Joystick_RawData_t *d)
{
    if (d == ((void*)0)) return;

    d->x = analogRead(JOYSTICK_X_CH);
    d->y = analogRead(JOYSTICK_Y_CH);
}

Joystick_Direction_t JOYSTICK_GET_DIR(void)
{
    Joystick_RawData_t r;
    JOYSTICK_READ_RAW(&r);

    u8 fwd = (r.y >= JOYSTICK_HIGH_THRESH) ? 1u : 0u;
    u8 bwd = (r.y <= JOYSTICK_LOW_THRESH ) ? 1u : 0u;
    u8 rgt = (r.x >= JOYSTICK_HIGH_THRESH) ? 1u : 0u;
    u8 lft = (r.x <= JOYSTICK_LOW_THRESH ) ? 1u : 0u;

    if (fwd && lft) return JOYSTICK_DIR_FORWARD_LEFT;
    if (fwd && rgt) return JOYSTICK_DIR_FORWARD_RIGHT;
    if (bwd && lft) return JOYSTICK_DIR_BACKWARD_LEFT;
    if (bwd && rgt) return JOYSTICK_DIR_BACKWARD_RIGHT;

    if (fwd) return JOYSTICK_DIR_FORWARD;
    if (bwd) return JOYSTICK_DIR_BACKWARD;
    if (lft) return JOYSTICK_DIR_LEFT;
    if (rgt) return JOYSTICK_DIR_RIGHT;

    return JOYSTICK_DIR_STOP;
}

u8 JOYSTICK_IsBtnPressed(void)
{
    return (GPIO_ReadPin(JOYSTICK_PB_PORT, JOYSTICK_PB_PIN) == GPIO_LOW) ? 1u : 0u;
}

