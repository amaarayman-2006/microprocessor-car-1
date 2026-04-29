#include "JOYSTICK.h"

/* =====================================================================
 *  JOYSTICK.c — joystick reading and 9-zone direction detection
 * ===================================================================== */

void JOYSTICK_INIT(void)
{
    adc_init(AVCC, DIV_128);

    /* Joystick push button: input with internal pull-up, active LOW. */
    GPIO_SetPinDirection(JOYSTICK_PB_PORT, JOYSTICK_PB_PIN, GPIO_INPUT);
    GPIO_WritePin       (JOYSTICK_PB_PORT, JOYSTICK_PB_PIN, GPIO_HIGH);
}

void JOYSTICK_READ_RAW(Joystick_RawData_t *pData)
{
    if (pData == ((void*)0)) return;

    pData->x = analogRead(JOYSTICK_X_CH);
    pData->y = analogRead(JOYSTICK_Y_CH);
}

Joystick_Direction_t JOYSTICK_GET_DIR(void)
{
    Joystick_RawData_t raw;
    JOYSTICK_READ_RAW(&raw);

    /* Direction flags. The south/backward side is intentionally checked with
     * >= HIGH threshold so south-east/south-west are detected before pure
     * east/west. */
    u8 north = (raw.y <= JOYSTICK_LOW_THRESH ) ? 1u : 0u;
    u8 south = (raw.y >= JOYSTICK_HIGH_THRESH) ? 1u : 0u;
    u8 east  = (raw.x >= JOYSTICK_HIGH_THRESH) ? 1u : 0u;
    u8 west  = (raw.x <= JOYSTICK_LOW_THRESH ) ? 1u : 0u;

    /* Diagonals first. This is the important part: a south-east or
     * south-west corner must not be swallowed as only RIGHT or LEFT. */
    if (north && west) return JOYSTICK_DIR_FORWARD_LEFT;
    if (north && east) return JOYSTICK_DIR_FORWARD_RIGHT;
    if (south && west) return JOYSTICK_DIR_BACKWARD_LEFT;    /* south-west */
    if (south && east) return JOYSTICK_DIR_BACKWARD_RIGHT;   /* south-east */

    if (north) return JOYSTICK_DIR_FORWARD;
    if (south) return JOYSTICK_DIR_BACKWARD;
    if (west ) return JOYSTICK_DIR_LEFT;
    if (east ) return JOYSTICK_DIR_RIGHT;

    return JOYSTICK_DIR_STOP;
}

u8 JOYSTICK_IsBtnPressed(void)
{
    return (GPIO_ReadPin(JOYSTICK_PB_PORT, JOYSTICK_PB_PIN) == GPIO_LOW) ? 1u : 0u;
}
