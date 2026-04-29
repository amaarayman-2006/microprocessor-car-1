#ifndef MOTOR_H
#define MOTOR_H

#include "GPIO_interface.h"
#include "JOYSTICK.h"

typedef enum
{
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_LEFT,
    MOTOR_RIGHT
} Motor_Direction_t;

typedef enum
{
    MOTOR_HALF_STOP = 0,
    MOTOR_HALF_FORWARD,
    MOTOR_HALF_BACKWARD
} Motor_HalfDir_t;

void MOTOR_Init                (void);

void MOTOR_Move                (Motor_Direction_t dir);

void MOTOR_Stop                (void);

void MOTOR_SetLeft             (Motor_HalfDir_t dir);
void MOTOR_SetRight            (Motor_HalfDir_t dir);

void MOTOR_Update_FromJoystick (void);

#endif

