#include "MOTOR.h"

#define LM_IN1_PORT   GPIO_PORTB
#define LM_IN1_PIN    GPIO_PIN4
#define LM_IN2_PORT   GPIO_PORTB
#define LM_IN2_PIN    GPIO_PIN5

#define RM_IN1_PORT   GPIO_PORTD
#define RM_IN1_PIN    GPIO_PIN1
#define RM_IN2_PORT   GPIO_PORTD
#define RM_IN2_PIN    GPIO_PIN0

#define KILL_PORT     GPIO_PORTB
#define KILL_PIN      GPIO_PIN1
#define KILL_ON       GPIO_HIGH
#define KILL_OFF      GPIO_LOW

#define PWM_PERIOD       16u
#define PWM_DUTY_INNER    8u

void MOTOR_Init(void)
{

    GPIO_SetPinDirection(LM_IN1_PORT, LM_IN1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(LM_IN2_PORT, LM_IN2_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(RM_IN1_PORT, RM_IN1_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(RM_IN2_PORT, RM_IN2_PIN, GPIO_OUTPUT);

    GPIO_SetPinDirection(KILL_PORT, KILL_PIN, GPIO_OUTPUT);
    GPIO_WritePin       (KILL_PORT, KILL_PIN, KILL_ON);

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

            MOTOR_SetLeft (MOTOR_HALF_STOP);
            MOTOR_SetRight(MOTOR_HALF_FORWARD);
            break;

        case MOTOR_RIGHT:

            MOTOR_SetLeft (MOTOR_HALF_FORWARD);
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

#define BTN_DEBOUNCE_COUNT   4u

void MOTOR_Update_FromJoystick(void)
{

    static u8 en  = 1u;
    static u8 btnSt      = 0u;
    static u8 btnLast     = 0u;
    static u8 dbCnt    = 0u;
    static u8 t       = 0u;

    u8 btn = JOYSTICK_IsBtnPressed();

    if (btn != btnLast)
    {

        dbCnt = 0u;
        btnLast  = btn;
    }
    else if (dbCnt < BTN_DEBOUNCE_COUNT)
    {

        dbCnt++;

        if (dbCnt == BTN_DEBOUNCE_COUNT)
        {

            u8 btnNow = btn;

            if (btnNow && !btnSt)
            {

                en = (en) ? 0u : 1u;
                GPIO_WritePin(KILL_PORT, KILL_PIN,
                              en ? KILL_ON : KILL_OFF);
            }
            btnSt = btnNow;
        }
    }

    if (!en)
    {
        MOTOR_Stop();
        return;
    }

    t++;
    if (t >= PWM_PERIOD) t = 0u;

    u8 inOn = (t < PWM_DUTY_INNER) ? 1u : 0u;

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

            MOTOR_SetRight(MOTOR_HALF_FORWARD);
            MOTOR_SetLeft (inOn ? MOTOR_HALF_FORWARD : MOTOR_HALF_STOP);
            break;

        case JOYSTICK_DIR_FORWARD_RIGHT:

            MOTOR_SetLeft (MOTOR_HALF_FORWARD);
            MOTOR_SetRight(inOn ? MOTOR_HALF_FORWARD : MOTOR_HALF_STOP);
            break;

        case JOYSTICK_DIR_BACKWARD_LEFT:

            MOTOR_SetRight(MOTOR_HALF_BACKWARD);
            MOTOR_SetLeft (inOn ? MOTOR_HALF_BACKWARD : MOTOR_HALF_STOP);
            break;

        case JOYSTICK_DIR_BACKWARD_RIGHT:

            MOTOR_SetLeft (MOTOR_HALF_BACKWARD);
            MOTOR_SetRight(inOn ? MOTOR_HALF_BACKWARD : MOTOR_HALF_STOP);
            break;

        case JOYSTICK_DIR_STOP:
        default:
            MOTOR_Stop();
            break;
    }
}

