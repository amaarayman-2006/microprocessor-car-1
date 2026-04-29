#ifndef BATTERY_H_
#define BATTERY_H_

#include "STD_Types.h"

#define BATTERY_ADC_CHANNEL          2u
#define BATTERY_R1_OHMS              10000UL
#define BATTERY_R2_OHMS              10000UL
#define BATTERY_VREF_MV              5000UL
#define BATTERY_ADC_MAX              1023UL

#define BATTERY_LOW_THRESHOLD_MV     6500UL
#define BATTERY_RECOVER_THRESHOLD_MV 6800UL

void BATTERY_Init          (void);
u16  BATTERY_ReadVoltage_mV(void);
u8   BATTERY_IsLow         (void);

#endif

