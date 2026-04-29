#include "MOTOR.h"
#include "JOYSTICK.h"
#include "BATTERY.h"
#include "LED.h"
#include "interface.h"

#define BATT_DIV   50u

int main(void)
{

    adc_init(AVCC, DIV_128);

    MOTOR_Init();
    JOYSTICK_INIT();
    BATTERY_Init();
    LED_Init();

    u8 bt = 0u;

    for (;;)
    {

        MOTOR_Update_FromJoystick();

        if (++bt >= BATT_DIV)
        {
            bt = 0u;

            if (BATTERY_IsLow())
            {

                LED_SetState(LED_STATE_ON);
            }
            else
            {

                LED_SetState(LED_STATE_OFF);
            }
        }
    }

    return 0;
}

