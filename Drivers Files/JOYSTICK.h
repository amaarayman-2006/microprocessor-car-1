#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "STD_Types.h"
#include "interface.h"
#include "GPIO_interface.h"

#define JOYSTICK_X_CH            0u
#define JOYSTICK_Y_CH            1u

#define JOYSTICK_PB_PORT         GPIO_PORTD
#define JOYSTICK_PB_PIN          GPIO_PIN2

#define JOYSTICK_ADC_MAX         1023u
#define JOYSTICK_CENTER          (JOYSTICK_ADC_MAX / 2u)
#define JOYSTICK_DEAD_ZONE       150u
#define JOYSTICK_HIGH_THRESH     (JOYSTICK_CENTER + JOYSTICK_DEAD_ZONE)
#define JOYSTICK_LOW_THRESH      (JOYSTICK_CENTER - JOYSTICK_DEAD_ZONE)

typedef struct
{
    u16 x;
    u16 y;
} Joystick_RawData_t;

typedef enum
{
    JOYSTICK_DIR_STOP = 0,
    JOYSTICK_DIR_FORWARD,
    JOYSTICK_DIR_BACKWARD,
    JOYSTICK_DIR_LEFT,
    JOYSTICK_DIR_RIGHT,
    JOYSTICK_DIR_FORWARD_LEFT,
    JOYSTICK_DIR_FORWARD_RIGHT,
    JOYSTICK_DIR_BACKWARD_LEFT,
    JOYSTICK_DIR_BACKWARD_RIGHT
} Joystick_Direction_t;

void                  JOYSTICK_INIT        (void);
void                  JOYSTICK_READ_RAW    (Joystick_RawData_t *pData);
Joystick_Direction_t  JOYSTICK_GET_DIR     (void);
u8                    JOYSTICK_IsBtnPressed(void);

#endif

