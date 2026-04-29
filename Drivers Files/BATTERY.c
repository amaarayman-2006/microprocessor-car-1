#include "BATTERY.h"
#include "interface.h"

static u8 lowFlag = 0u;

void BATTERY_Init(void)
{

    lowFlag = 0u;
}

u16 BATTERY_ReadVoltage_mV(void)
{

    u16 adc = analogRead((u8)BATTERY_ADC_CHANNEL);

    u32 vAdc  = ((u32)adc * (u32)BATTERY_VREF_MV) / (u32)BATTERY_ADC_MAX;
    u32 vBat = (vAdc * (u32)(BATTERY_R1_OHMS + BATTERY_R2_OHMS))
                    / (u32)BATTERY_R2_OHMS;

    return (u16)vBat;
}

u8 BATTERY_IsLow(void)
{
    u16 v = BATTERY_ReadVoltage_mV();

    if (lowFlag)
    {

        if (v >= BATTERY_RECOVER_THRESHOLD_MV)
        {
            lowFlag = 0u;
        }
    }
    else
    {

        if (v <= BATTERY_LOW_THRESHOLD_MV)
        {
            lowFlag = 1u;
        }
    }

    return lowFlag;
}

