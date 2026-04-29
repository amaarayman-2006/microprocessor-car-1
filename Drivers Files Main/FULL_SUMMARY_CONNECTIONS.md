# Full Summary — Battery A3 + D7 Low-Battery LED Fix

## What changed

This version was made because the LED stayed ON all the time with the previous A2/D8 version.

Changes:

- `LED.c` is still removed.
- `LED.h` is still removed.
- The LED is still controlled only inside `BATTERY.c`.
- Battery ADC input changed from **A2** to **A3 / PC3 / ADC3**.
- Low-battery LED output changed from **D8** to **D7 / PD7**.
- The ADC decision now uses a direct raw threshold: **ADC <= 546 means battery <= 8V**.
- The battery ADC pin is explicitly configured as input with pull-up OFF.
- The LED pin is explicitly configured as output and forced LOW during startup.
- Extra averaging was added for a stable reading.

## Final logic

```text
Battery voltage <= 8.0V  -> D7 HIGH -> LED ON
Battery voltage >  8.0V  -> D7 LOW  -> LED OFF
```

## Why I changed the pins

The previous code already wrote D8 LOW when the calculated battery voltage was above 8V. If the LED stayed ON at 12V, the problem was likely one of these:

1. D8/PB0 was wired to the wrong physical pin.
2. The LED was connected active-low, from +5V to the pin.
3. The A2 divider point was not actually reaching about 4V at a 12V battery.
4. A2 or D8 had a wiring conflict on the real board.

So this fix moves the battery monitor to clean unused pins:

| Function | New pin |
|---|---|
| Battery divider midpoint | A3 / PC3 / ADC3 |
| Low-battery LED output | D7 / PD7 |

## Voltage divider with 10k resistors

Use **three 10kΩ resistors**:

```text
Battery +  ---- 10kΩ ---- 10kΩ ----+---- A3 / PC3 / ADC3
                                    |
                                  10kΩ
                                    |
Battery - / GND --------------------+---- MCU GND
```

Equivalent divider:

```text
R1 top    = 20kΩ
R2 bottom = 10kΩ
```

Formula:

```text
A3 voltage = Battery voltage × R2 / (R1 + R2)
A3 voltage = Battery voltage × 10000 / 30000
A3 voltage = Battery voltage / 3
```

Expected voltages:

| Real battery voltage | Voltage on A3 | ADC value approx. |
|---:|---:|---:|
| 12V | 4.00V | 818 |
| 8V | 2.67V | 546 |
| 7V | 2.33V | 477 |

## LED connection

Connect the LED like this:

```text
D7 / PD7 ---- 330Ω to 1kΩ resistor ---- LED anode (+)
LED cathode (-) ------------------------ GND
```

Do **not** connect it like this:

```text
5V ---- resistor ---- LED ---- D7
```

That is active-low wiring and it will behave backwards.

## Important physical pins

If you use an Arduino UNO header:

```text
A3 = Arduino analog header A3
D7 = Arduino digital header D7
```

If you use a bare ATmega328P DIP chip:

```text
A3 / PC3 = physical pin 26
D7 / PD7 = physical pin 13
```

## Meter test at 12V battery

Before running the full car, test this:

```text
A3 to GND should be about 4.0V
D7 to GND should be about 0V
LED should be OFF
```

If A3 is 0V or floating, the code will think the battery is low and the LED will stay ON.

If D7 is 0V but the LED is still ON, the LED is wired active-low or connected to the wrong pin.

## Main code

`main.c` only calls the battery driver:

```c
BATTERY_Init();
BATTERY_UpdateLowLed();
```

No separate LED driver is used.
