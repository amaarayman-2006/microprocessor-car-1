#ifndef BATTERY_H_
#define BATTERY_H_

#include "STD_Types.h"

/* =========================================================================
 *  BATTERY.h — 12V battery monitor + low-battery LED control
 *
 *  IMPORTANT FIX VERSION:
 *      The low-battery indicator LED is controlled INSIDE this battery driver.
 *      There is no LED.c / LED.h driver.
 *
 *  New pins chosen to avoid the "LED always ON" problem:
 *      Battery sense input -> A3 / PC3 / ADC3
 *      Low-battery LED    -> D7 / PD7
 *
 *  Why changed from A2/D8?
 *      If D8/PB0 wiring, shield wiring, or a wrong physical pin causes the LED
 *      to stay ON, D7/PD7 is a clean normal digital output pin not used by the
 *      motors, joystick, UART, SPI, or ADC inputs in this project.
 *      A3/ADC3 is also unused by the joystick, so the battery reading is isolated.
 *
 *  Battery divider using only 10k resistors:
 *      Use two 10k resistors in series for the TOP side, and one 10k resistor
 *      for the BOTTOM side.
 *
 *      Battery +  ---- 10k ---- 10k ----+---- A3 / PC3 / ADC3
 *                                        |
 *                                      10k
 *                                        |
 *      Battery - / GND ------------------+---- MCU GND
 *
 *  Equivalent values:
 *      R1/top    = 20k  (battery+ to A3)
 *      R2/bottom = 10k  (A3 to GND)
 *
 *  With R1 = 20k and R2 = 10k:
 *      V_ADC = V_BATTERY / 3
 *      12.0V battery -> about 4.000V at A3
 *       8.0V battery -> about 2.667V at A3
 *
 *  Required behaviour:
 *      Battery voltage <= 8.0V  -> D7 HIGH -> LED ON
 *      Battery voltage  > 8.0V  -> D7 LOW  -> LED OFF
 *
 *  Hardware LED wiring for this code:
 *      D7 / PD7 ---- 330R..1k ---- LED anode (+)
 *      LED cathode (-) ----------- GND
 *
 *  If your LED is connected from +5V to D7, it is active-low and it will behave
 *  backwards. Do not use that wiring with this code.
 * ========================================================================= */

#define BATTERY_ADC_CHANNEL          3u              /* PC3 / ADC3 / A3 */
#define BATTERY_ADC_PORT             GPIO_PORTC
#define BATTERY_ADC_PIN              GPIO_PIN3

#define BATTERY_LED_PORT             GPIO_PORTD
#define BATTERY_LED_PIN              GPIO_PIN7       /* Arduino D7 = PD7 */

#define BATTERY_R1_OHMS              20000UL         /* top: two 10k in series */
#define BATTERY_R2_OHMS              10000UL         /* bottom: one 10k to GND */
#define BATTERY_VREF_MV              5000UL          /* AVCC reference in mV */
#define BATTERY_ADC_MAX              1023UL          /* 10-bit ADC max */

#define BATTERY_LOW_THRESHOLD_MV     8000UL          /* <= 8.0V -> LED ON */
#define BATTERY_SAMPLE_COUNT         16u             /* stronger averaging */

/* Raw ADC threshold for the same condition:
 * 8V battery -> A3 = 8/3 = 2.6667V
 * ADC = 2.6667 / 5.0 * 1023 = about 546
 *
 * We use the raw ADC decision because it removes any integer conversion mistake.
 */
#define BATTERY_LOW_ADC_THRESHOLD    546u

void BATTERY_Init             (void);
u16  BATTERY_ReadRawAdc       (void);
u16  BATTERY_ReadVoltage_mV   (void);
u8   BATTERY_IsLow            (void);
void BATTERY_UpdateLowLed     (void);

/* Useful while testing with a meter: forces the configured LED pin OFF/ON. */
void BATTERY_LedOff           (void);
void BATTERY_LedOn            (void);

#endif /* BATTERY_H_ */
