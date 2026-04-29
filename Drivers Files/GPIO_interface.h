#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

#include "STD_Types.h"

#define GPIO_INPUT      0
#define GPIO_OUTPUT     1

#define GPIO_LOW        0
#define GPIO_HIGH       1

#define GPIO_PORTB      0
#define GPIO_PORTC      1
#define GPIO_PORTD      2

#define GPIO_PIN0       0
#define GPIO_PIN1       1
#define GPIO_PIN2       2
#define GPIO_PIN3       3
#define GPIO_PIN4       4
#define GPIO_PIN5       5
#define GPIO_PIN6       6
#define GPIO_PIN7       7

void GPIO_SetPinDirection(u8 Port, u8 Pin, u8 Direction);
void GPIO_WritePin       (u8 Port, u8 Pin, u8 Value);
u8   GPIO_ReadPin        (u8 Port, u8 Pin);

#endif

