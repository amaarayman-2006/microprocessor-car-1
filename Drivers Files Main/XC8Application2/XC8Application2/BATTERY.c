#include "BATTERY.h"
#include "interface.h"        /* adc_init, analogRead */
#include "GPIO_interface.h"  /* GPIO control */

/* =====================================================================
 *  BATTERY.c — battery voltage monitor for 12V battery on A3
 *
 *  This version does everything inside the battery driver:
 *      - Reads the battery divider from A3 / PC3 / ADC3.
 *      - Calculates the real battery voltage in millivolts.
 *      - Controls the low-battery LED directly on D7 / PD7.
 *      - No LED.c / LED.h driver is used.
 *      - Threshold is 8.0V or less.
 *      - No latch is used: above 8.0V the LED turns OFF again.
 *      - Decision is based on raw ADC threshold to avoid conversion bugs.
 * ===================================================================== */

void BATTERY_LedOff(void)
{
    GPIO_WritePin(BATTERY_LED_PORT, BATTERY_LED_PIN, GPIO_LOW);   /* D7 = 0V */
}

void BATTERY_LedOn(void)
{
    GPIO_WritePin(BATTERY_LED_PORT, BATTERY_LED_PIN, GPIO_HIGH);  /* D7 = 5V */
}

void BATTERY_Init(void)
{
    /* Make sure ADC is enabled with AVCC reference. Safe even if main already
     * called adc_init(). */
    adc_init(AVCC, DIV_128);

    /* Battery ADC pin A3 / PC3 must be INPUT with pull-up OFF.
     * This is important: a pull-up on the ADC pin can corrupt the divider. */
    GPIO_SetPinDirection(BATTERY_ADC_PORT, BATTERY_ADC_PIN, GPIO_INPUT);
    GPIO_WritePin       (BATTERY_ADC_PORT, BATTERY_ADC_PIN, GPIO_LOW);

    /* Low-battery LED output on D7 / PD7. Start OFF. */
    GPIO_SetPinDirection(BATTERY_LED_PORT, BATTERY_LED_PIN, GPIO_OUTPUT);
    BATTERY_LedOff();
}

u16 BATTERY_ReadRawAdc(void)
{
    u8 i;
    u32 adc_sum = 0UL;

    /* Extra first read discarded. This protects the battery reading from the
     * previous joystick ADC channel. */
    (void)analogRead((u8)BATTERY_ADC_CHANNEL);

    for (i = 0u; i < BATTERY_SAMPLE_COUNT; i++)
    {
        adc_sum += (u32)analogRead((u8)BATTERY_ADC_CHANNEL);
    }

    return (u16)((adc_sum + ((u32)BATTERY_SAMPLE_COUNT / 2UL)) /
                 (u32)BATTERY_SAMPLE_COUNT);
}

u16 BATTERY_ReadVoltage_mV(void)
{
    u32 adc_avg = (u32)BATTERY_ReadRawAdc();

    /*
     * V_adc_mV  = ADC * VREF_mV / 1023
     * V_batt_mV = V_adc_mV * (R1 + R2) / R2
     *
     * With R1 = 20k and R2 = 10k:
     *      V_batt_mV = V_adc_mV * 3
     */
    u32 v_adc_mV  = (adc_avg * (u32)BATTERY_VREF_MV + ((u32)BATTERY_ADC_MAX / 2UL))
                    / (u32)BATTERY_ADC_MAX;

    u32 v_batt_mV = (v_adc_mV * (u32)(BATTERY_R1_OHMS + BATTERY_R2_OHMS)
                    + ((u32)BATTERY_R2_OHMS / 2UL)) / (u32)BATTERY_R2_OHMS;

    if (v_batt_mV > 65535UL)
    {
        return 65535u;
    }

    return (u16)v_batt_mV;
}

u8 BATTERY_IsLow(void)
{
    u16 adc = BATTERY_ReadRawAdc();

    /* Required behaviour using the real divider math:
     *   battery <= 8.0V -> A3 <= about 2.667V -> ADC <= about 546 -> LED ON
     *   battery  > 8.0V -> ADC >  546        -> LED OFF
     */
    return (adc <= (u16)BATTERY_LOW_ADC_THRESHOLD) ? 1u : 0u;
}

void BATTERY_UpdateLowLed(void)
{
    if (BATTERY_IsLow())
    {
        BATTERY_LedOn();
    }
    else
    {
        BATTERY_LedOff();
    }
}
