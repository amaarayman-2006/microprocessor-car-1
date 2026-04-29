#ifndef LED_H_
#define LED_H_

#include "STD_Types.h"
#include "GPIO_interface.h"

#define LED_LOW_BATT_PORT   GPIO_PORTB
#define LED_LOW_BATT_PIN    GPIO_PIN0

typedef enum
{
    LED_STATE_OFF = 0,
    LED_STATE_ON  = 1
} LED_State_t;

void LED_Init    (void);
void LED_SetState(LED_State_t state);
void LED_Toggle  (void);

#endif

