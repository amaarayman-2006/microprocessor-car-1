#ifndef TYPES_H
#define TYPES_H

#include "STD_Types.h"

typedef enum
{
    AREF      = 0,
    AVCC      = 1,
    INTERNAL  = 2
} voltage_ref;

typedef enum
{
    ADC_CH0 = 0,
    ADC_CH1,
    ADC_CH2,
    ADC_CH3,
    ADC_CH4,
    ADC_CH5,
    ADC_CH6,
    ADC_CH7
} adc_channel;

typedef enum
{
    DIV_2 = 0,
    DIV_4,
    DIV_8,
    DIV_16,
    DIV_32,
    DIV_64,
    DIV_128
} pre_Scalar;

#endif

