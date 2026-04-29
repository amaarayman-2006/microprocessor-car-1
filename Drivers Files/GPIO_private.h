#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

#include "STD_Types.h"

#define PINB    (*((volatile u8*)0x23))
#define DDRB    (*((volatile u8*)0x24))
#define PORTB   (*((volatile u8*)0x25))

#define PINC    (*((volatile u8*)0x26))
#define DDRC    (*((volatile u8*)0x27))
#define PORTC   (*((volatile u8*)0x28))

#define PIND    (*((volatile u8*)0x29))
#define DDRD    (*((volatile u8*)0x2A))
#define PORTD   (*((volatile u8*)0x2B))

#endif

