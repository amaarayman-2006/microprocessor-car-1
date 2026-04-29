# 🛠️ Hardware Bill of Materials (BOM)

This document outlines the hardware components required to build and operate the differential drive robot chassis. 

## 🧠 Core Processing & Control
The system is built around the ATmega328p architecture, utilizing a standard joystick for analog directional input.

| Component | Qty | Description / Specifications |
| :--- | :---: | :--- |
| **ATmega328p** | 1 | Main component of the project. It can be found inside of an Arduino Uno. Handles logic, processing and ADC readings. |
| **Joystick X-Y Module** | 1 | Dual-axis analog potentiometers with a tactile push-button (used for directional control and kill-switch). |

## ⚙️ Actuation & Drive System
The drivetrain relies on a dual H-bridge setup to independently control the speed and direction of the two high-speed DC motors.

| Component | Qty | Description / Specifications |
| :--- | :---: | :--- |
| **L298 Motor Driver** | 1 | Dual H-bridge module for driving high-current loads via microcontroller logic. |
| **DC Motors** | 2 | 600 RPM gear motors providing differential drive to the chassis. |
| **Robot Chassis** | 1 | Base mechanical frame for mounting motors, electronics, and power supply. |

## 🔋 Power Management
A series-connected lithium-ion pack provides an 11.1V nominal voltage to efficiently power the L298 driver and motors, while the Arduino regulates its own 5V logic.

| Component | Qty | Description / Specifications |
| :--- | :---: | :--- |
| **3.7V Li-ion Batteries** | 3 | Wired in series to provide 11.1V for motor driving and logic power. |
| **Power Switch** | 1 | Master SPST switch to easily isolate the battery pack from the active circuit. |

## 🔌 Prototyping & Connectivity
Standard prototyping components for status indication and signal routing.

| Component | Qty | Description / Specifications |
| :--- | :---: | :--- |
| **Breadboard** | 1 | Half-size board for routing low-voltage logic and indicator circuits. |
| **LEDs** | 2 | Visual status indicators ( Green for POWER ON, Red is when battery voltage is below 8V ). |
| **Resistors** | Set | Used for LED current-limiting (typically 220Ω - 330Ω) and potential pull-up/down logic. |
| **Male-to-Male Jumpers** | Set | Used for routing signals across the breadboard and Arduino. |
| **Crocodile Jumpers** | Set | Used for temporary, secure connections to battery terminals and motor leads. |

---
*Note: Ensure all ground connections (Arduino GND, L298 GND, and Battery GND) are tied together to establish a common reference voltage for the control logic.*
