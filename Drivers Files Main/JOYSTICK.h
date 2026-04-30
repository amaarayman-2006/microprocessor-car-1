#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "STD_Types.h"
#include "interface.h"          /* ADC API: analogRead */
#include "GPIO_interface.h"     /* GPIO: joystick button */

/* =====================================================================
 *  Joystick wiring on Arduino UNO / ATmega328P
 *
 *      VRX  -> A0 / PC0 / ADC0
 *      VRY  -> A1 / PC1 / ADC1
 *      SW   -> D2 / PD2, active LOW with internal pull-up
 *      VCC  -> +5V
 *      GND  -> GND
 *
 *  Direction convention used by the firmware:
 *      North / Forward  = Y ADC low
 *      South / Backward = Y ADC high
 *      West  / Left     = X ADC low
 *      East  / Right    = X ADC high
 *
 *  Fix in this version:
 *      The dead-zone was narrowed from +/-150 to +/-120. This makes the two
 *      south diagonals (south-east and south-west) easier to detect on common
 *      joystick modules that do not always reach a perfect 0/1023 corner.
 * ===================================================================== */

#define JOYSTICK_X_CH            0u    /* A0 = PC0 = ADC0 */
#define JOYSTICK_Y_CH            1u    /* A1 = PC1 = ADC1 */

#define JOYSTICK_PB_PORT         GPIO_PORTD
#define JOYSTICK_PB_PIN          GPIO_PIN2     /* D2 = PD2 */

#define JOYSTICK_ADC_MAX         1023u
#define JOYSTICK_CENTER          (JOYSTICK_ADC_MAX / 2u)                  /* 511 */
#define JOYSTICK_DEAD_ZONE       120u                                     /* +/-120 */
#define JOYSTICK_HIGH_THRESH     (JOYSTICK_CENTER + JOYSTICK_DEAD_ZONE)   /* 631 */
#define JOYSTICK_LOW_THRESH      (JOYSTICK_CENTER - JOYSTICK_DEAD_ZONE)   /* 391 */

typedef struct
{
    u16 x;   /* 0 = left/west, 1023 = right/east */
    u16 y;   /* 0 = forward/north, 1023 = backward/south */
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
    JOYSTICK_DIR_BACKWARD_LEFT,    /* south-west */
    JOYSTICK_DIR_BACKWARD_RIGHT    /* south-east */
} Joystick_Direction_t;

void                  JOYSTICK_INIT        (void);
void                  JOYSTICK_READ_RAW    (Joystick_RawData_t *pData);
Joystick_Direction_t  JOYSTICK_GET_DIR     (void);
u8                    JOYSTICK_IsBtnPressed(void);

#endif /* JOYSTICK_H_ */
