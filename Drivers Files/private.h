#ifndef PRIVATE_H
#define PRIVATE_H

#include "types.h"

#define ADCL    (*((volatile u8*)0x78))
#define ADCH    (*((volatile u8*)0x79))
#define ADCSRA  (*((volatile u8*)0x7A))
#define ADMUX   (*((volatile u8*)0x7C))

#define ADPS0   0
#define ADPS1   1
#define ADPS2   2
#define ADIE    3
#define ADIF    4
#define ADATE   5
#define ADSC    6
#define ADEN    7

#define MUX0    0
#define MUX1    1
#define MUX2    2
#define MUX3    3
#define ADLAR   5
#define REFS0   6
#define REFS1   7

#define setBIT(reg, pin)    ((reg) |=  (1U << (pin)))
#define clearBIT(reg, pin)  ((reg) &= ~(1U << (pin)))
#define readBIT(reg, pin)   (((reg) & (1U << (pin))) != 0U)

#endif

