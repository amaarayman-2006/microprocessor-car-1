#ifndef MOTOR_H
#define MOTOR_H

#include "GPIO_interface.h"
#include "JOYSTICK.h"

/* =====================================================================
 *  MOTOR.h — two-motor robot car interface
 *
 *  Two API levels:
 *    1) Motor_Direction_t: whole-car commands.
 *    2) Motor_HalfDir_t  : one-wheel commands used internally.
 *
 *  Relay switch removed:
 *    There is no relay/kill-switch API in this module now.
 * ===================================================================== */

/* Whole-car movement commands */
typedef enum
{
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_LEFT,
    MOTOR_RIGHT,
    MOTOR_FORWARD_LEFT,
    MOTOR_FORWARD_RIGHT,
    MOTOR_BACKWARD_LEFT,
    MOTOR_BACKWARD_RIGHT
} Motor_Direction_t;

/* Single-motor commands */
typedef enum
{
    MOTOR_HALF_STOP = 0,
    MOTOR_HALF_FORWARD,
    MOTOR_HALF_BACKWARD
} Motor_HalfDir_t;

/* ---------------- APIs ---------------- */
void MOTOR_Init                (void);
void MOTOR_Move                (Motor_Direction_t dir);
void MOTOR_Stop                (void);
void MOTOR_SetLeft             (Motor_HalfDir_t dir);
void MOTOR_SetRight            (Motor_HalfDir_t dir);
void MOTOR_Update_FromJoystick (void);

#endif /* MOTOR_H */
