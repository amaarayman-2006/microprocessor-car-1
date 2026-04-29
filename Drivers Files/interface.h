#ifndef INTERFACE_H
#define INTERFACE_H

#include "types.h"

void adc_init(voltage_ref vref, pre_Scalar Scalar);

u16  analogRead(u8 channel);

#endif

