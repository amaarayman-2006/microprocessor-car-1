#include "MOTOR.h"

/* =====================================================================
 *  MOTOR.c — motor driver for two-wheel robot car, L298N/L293D style
 *
 *  Wiring used by this firmware:
 *      Left motor : IN1 -> D12/PB4, IN2 -> D13/PB5
 *      Right motor: IN3 -> D1/PD1,  IN4 -> D0/PD0
 *
 *  Fix in this version:
 *      South-west and south-east outputs were reversed on the real car, so the
 *      two reverse diagonal motor actions were swapped only.
 *
 *      SW / BACKWARD_LEFT  = right wheel backward, left wheel stopped
 *      SE / BACKWARD_RIGHT = left wheel backward, right wheel stopped
 * ===================================================================== */

/* Left motor */
#define LM_IN1_PORT   GPIO_PORTB
#define LM_IN1_PIN    GPIO_PIN4   /* D12 = PB4 */
#define LM_IN2_PORT   GPIO_PORTB
#define LM_IN2_PIN    GPIO_PIN5   /* D13 = PB5 */

/* Right motor */
#define RM_IN1_PORT   GPIO_PORTD
#define RM_IN1_PIN    GPIO_PIN1   /* D1 = PD1 = IN3 */
#define RM_IN2_PORT   GPIO_PORTD
#define RM_IN2_PIN    GPIO_PIN0   /* D0 = PD0 = IN4 */

void MOTOR_Init(void)
{
    GPIO_SetPinDirection(LM_IN1_PORT, LM_IN1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(LM_IN2_PORT, LM_IN2_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(RM_IN1_PORT, RM_IN1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(RM_IN2_PORT, RM_IN2_PIN, GPIO_OUTPUT);

    MOTOR_Stop();
}

void MOTOR_SetLeft(Motor_HalfDir_t dir)
{
    switch (dir)
    {
        case MOTOR_HALF_FORWARD:
            GPIO_WritePin(LM_IN1_PORT, LM_IN1_PIN, GPIO_LOW );
            GPIO_WritePin(LM_IN2_PORT, LM_IN2_PIN, GPIO_HIGH);
            break;

        case MOTOR_HALF_BACKWARD:
            GPIO_WritePin(LM_IN1_PORT, LM_IN1_PIN, GPIO_HIGH);
            GPIO_WritePin(LM_IN2_PORT, LM_IN2_PIN, GPIO_LOW );
            break;

        case MOTOR_HALF_STOP:
        default:
            GPIO_WritePin(LM_IN1_PORT, LM_IN1_PIN, GPIO_LOW);
            GPIO_WritePin(LM_IN2_PORT, LM_IN2_PIN, GPIO_LOW);
            break;
    }
}

void MOTOR_SetRight(Motor_HalfDir_t dir)
{
    switch (dir)
    {
        case MOTOR_HALF_FORWARD:
            GPIO_WritePin(RM_IN1_PORT, RM_IN1_PIN, GPIO_HIGH);
            GPIO_WritePin(RM_IN2_PORT, RM_IN2_PIN, GPIO_LOW );
            break;

        case MOTOR_HALF_BACKWARD:
            GPIO_WritePin(RM_IN1_PORT, RM_IN1_PIN, GPIO_LOW );
            GPIO_WritePin(RM_IN2_PORT, RM_IN2_PIN, GPIO_HIGH);
            break;

        case MOTOR_HALF_STOP:
        default:
            GPIO_WritePin(RM_IN1_PORT, RM_IN1_PIN, GPIO_LOW);
            GPIO_WritePin(RM_IN2_PORT, RM_IN2_PIN, GPIO_LOW);
            break;
    }
}

void MOTOR_Move(Motor_Direction_t dir)
{
    switch (dir)
    {
        case MOTOR_FORWARD:
            MOTOR_SetLeft (MOTOR_HALF_FORWARD);
            MOTOR_SetRight(MOTOR_HALF_FORWARD);
            break;

        case MOTOR_BACKWARD:
            MOTOR_SetLeft (MOTOR_HALF_BACKWARD);
            MOTOR_SetRight(MOTOR_HALF_BACKWARD);
            break;

        case MOTOR_LEFT:
            MOTOR_SetLeft (MOTOR_HALF_BACKWARD);
            MOTOR_SetRight(MOTOR_HALF_FORWARD);
            break;

        case MOTOR_RIGHT:
            MOTOR_SetLeft (MOTOR_HALF_FORWARD);
            MOTOR_SetRight(MOTOR_HALF_BACKWARD);
            break;

        case MOTOR_FORWARD_LEFT:
            MOTOR_SetLeft (MOTOR_HALF_STOP);
            MOTOR_SetRight(MOTOR_HALF_FORWARD);
            break;

        case MOTOR_FORWARD_RIGHT:
            MOTOR_SetLeft (MOTOR_HALF_FORWARD);
            MOTOR_SetRight(MOTOR_HALF_STOP);
            break;

        case MOTOR_BACKWARD_LEFT:      /* south-west */
            MOTOR_SetLeft (MOTOR_HALF_STOP);
            MOTOR_SetRight(MOTOR_HALF_BACKWARD);
            break;

        case MOTOR_BACKWARD_RIGHT:     /* south-east */
            MOTOR_SetLeft (MOTOR_HALF_BACKWARD);
            MOTOR_SetRight(MOTOR_HALF_STOP);
            break;

        case MOTOR_STOP:
        default:
            MOTOR_Stop();
            break;
    }
}

void MOTOR_Stop(void)
{
    MOTOR_SetLeft (MOTOR_HALF_STOP);
    MOTOR_SetRight(MOTOR_HALF_STOP);
}

void MOTOR_Update_FromJoystick(void)
{
    switch (JOYSTICK_GET_DIR())
    {
        case JOYSTICK_DIR_FORWARD:
            MOTOR_Move(MOTOR_FORWARD);
            break;

        case JOYSTICK_DIR_BACKWARD:
            MOTOR_Move(MOTOR_BACKWARD);
            break;

        case JOYSTICK_DIR_LEFT:
            MOTOR_Move(MOTOR_LEFT);
            break;

        case JOYSTICK_DIR_RIGHT:
            MOTOR_Move(MOTOR_RIGHT);
            break;

        case JOYSTICK_DIR_FORWARD_LEFT:
            MOTOR_Move(MOTOR_FORWARD_LEFT);
            break;

        case JOYSTICK_DIR_FORWARD_RIGHT:
            MOTOR_Move(MOTOR_FORWARD_RIGHT);
            break;

        case JOYSTICK_DIR_BACKWARD_LEFT:
            MOTOR_Move(MOTOR_BACKWARD_LEFT);     /* south-west */
            break;

        case JOYSTICK_DIR_BACKWARD_RIGHT:
            MOTOR_Move(MOTOR_BACKWARD_RIGHT);    /* south-east */
            break;

        case JOYSTICK_DIR_STOP:
        default:
            MOTOR_Stop();
            break;
    }
}
