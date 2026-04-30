/* =====================================================================
 *  main.c — main application for ATmega328P robot car
 * ===================================================================== */

#include "MOTOR.h"
#include "JOYSTICK.h"
#include "BATTERY.h"
#include "interface.h"   /* adc_init */

/* Check battery often enough that the LED reacts quickly, but not on every
 * motor-control loop iteration. */
#define BATT_CHECK_DIVIDER   10u

int main(void)
{
    adc_init(AVCC, DIV_128);

    MOTOR_Init();
    JOYSTICK_INIT();
    BATTERY_Init();

    /* Immediate startup check:
     * if the battery is already <= 8.0V, D7 turns HIGH immediately. */
    BATTERY_UpdateLowLed();

    u8 batt_tick = 0u;

    for (;;)
    {
        MOTOR_Update_FromJoystick();

        if (++batt_tick >= BATT_CHECK_DIVIDER)
        {
            batt_tick = 0u;
            BATTERY_UpdateLowLed();
        }
    }

    return 0;
}
