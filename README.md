# Wired Joystick Car Using ATmega328p
![C](https://img.shields.io/badge/Language-C-%23A8B9CC.svg?style=for-the-badge&logo=c&logoColor=white)
![Microchip](https://img.shields.io/badge/MCU-ATmega328p-%23005151.svg?style=for-the-badge&logo=microchip&logoColor=white)
![AVR](https://img.shields.io/badge/Architecture-AVR-%23005A9C.svg?style=for-the-badge)
![GitHub License](https://img.shields.io/github/license/amaarayman-2006/microprocessor-car-1?style=for-the-badge&color=blue)

This project is a part of "Principles of Microprocessors" course in the study plan for **Electric Power and Machines (EPM)** engineering students at Faculty of Engineering, Alexandria University.

This project is about making a functional car with a joystick wired controller using ATmega328P and applying our own drivers.

## Objective

Our objective was to build a functional car that can be controlled using wired joystick. The car also have a battery indicator function that detects if the overall voltage of the batteries drops to a certain level which is 8V as the motors can function in voltages between 7:12 VDC.

## Overview

<img src="Car%20without%20cover.png" width="500" alt="Car without cover">
<img src="Car%20with%20cover.png" width="400" alt="Car with cover">

## Drivers Software

### MCAL Layer Modules :
    |── GPIO
          |── GPIO_interface.h
          |── GPIO_private.h
          |── GPIO_config.h
          └── GPIO_program.c

    |── ADC
          |── ADC_interface.h
          |── ADC_private.h
          |── ADC_types.h
          └── ADC_program.c
          
    └── Global Files
          |── STD_Types.h
          └── BIT_MATH.h

### HAL Layer Modules :
    |── Joystick
          |── JOYSTICK.h
          └── JOYSTICK.c
          
    |── Motors
          |── MOTOR.h
          └── MOTOR.c

    |── Battery
          |── BATTERY.h
          └── BATTERY.c

### Application Layer Module :
    └── main.c

**N.B :** All the drivers modules and files can be accessed [**here**](Drivers%20Files%20Main/XC8Application2/XC8Application2)

## Hardware

You can check the hardware components list [**here**](Hardware%20Components.md)

<img src="https://sdvelectronica.com.ar/wp-content/uploads/2019/05/ATMEGA328P.jpg" alt="Diagram" width="400">

## Team Members
1. Amaar Ayman
2. Ahmed Nagy
3. Omar Wael
4. Seif Raafat
5. Morsy Ahmed
6. Youssef Moussa
