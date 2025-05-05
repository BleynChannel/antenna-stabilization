# Antenna Turn MCU Firmware

![Project Cover](https://github.com/BleynChannel/antenna-stabilization/blob/main/assets/img/Antenna.png)

[![English](https://img.shields.io/badge/English-blue)](README_EN.md)
[![Russian](https://img.shields.io/badge/Русский-blue)](README.md)

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Description

This project is firmware for a microcontroller (ESP32) that controls an antenna with automatic and manual positioning capabilities. The system implements auto-stabilization of the antenna at a dynamic angle - the antenna automatically tracks and maintains the desired direction using data from the compass and/or autopilot (ArduPilot).

The system supports operation with a compass (AS5600), manual control (Serial), and integration with MAVLink to receive parameters from external devices (e.g., ArduPilot-based autopilots).

## Key Features
- Automatic stabilization of the antenna at a dynamic angle (auto-targeting)
- Antenna control via servo motors with smooth positioning
- Angle acquisition from magnetic encoder (AS5600)
- Manual control of antenna position (hardware or software Serial)
- MAVLink data reception and processing (VFR_HUD, Attitude)
- Flexible architecture: separate classes for logging, logic, MAVLink, compass, and manual control
- Support for expansion with additional sensor types and protocols

## Connection
- **MCU:** ESP32 (or compatible)
- **Compass:** AS5600 (I2C)
- **Servo Motors:** Connected to pins defined in `const.h`
- **Manual Control:** Serial (see configuration)
- **MAVLink:** Direct connection to autopilot (e.g., SpeedyBee F405 V4, Matek, Pixhawk, etc. running ArduPilot) via RX/TX ports

## Connection Diagram (Pinout)

| Function              | ESP32 GPIO | Description/Notes           |
|-----------------------|------------|-----------------------------|
| Servo PWM (Main)      |    27      | ANTENNA_MAIN_PIN            |
| Servo PWM (Second)    |    26      | ANTENNA_SECOND_PIN          |
| Servo Min Pulse       |   1470     | ANTENNA_MAIN_MIN_FREQURENCE |
| Servo Max Pulse       |   1540     | ANTENNA_MAIN_MAX_FREQURENCE |
| Compass SCL           |    22      | COMPASS_SCL_PIN (I2C SCL)   |
| Compass SDA           |    21      | COMPASS_SDA_PIN (I2C SDA)   |
| Compass Direction     |     0      | COMPASS_DIRECTION           |
| Manual RX             |    10      | MANUAL_RX_PIN               |
| Manual TX             |    11      | MANUAL_TX_PIN               |
| MAVLink RX            |    16      | MAVLINK_RX_PIN              |
| MAVLink TX            |    17      | MAVLINK_TX_PIN              |

> All pin values and parameters can be changed in the `const.h` file.
>
> **MAVLink RX/TX**: Direct connection to UART ports of flight controllers (e.g., SpeedyBee F405 V4, Matek, Pixhawk, etc.) running ArduPilot.
>
> **Manual RX/TX**: Direct connection to UART ports for manual control (e.g., computer, other controller).

```
ESP32
   +----------------------------+
   |      [27] Main Servo PWM   |----> Main servo
   |      [26] Second Servo PWM |----> Secondary servo
   |      [22] SCL              |----> AS5600 SCL
   |      [21] SDA              |----> AS5600 SDA
   |      [16] MAVLink RX       |<---- Autopilot TX (ArduPilot)
   |      [17] MAVLink TX       |----> Autopilot RX (ArduPilot)
   |      [10] Manual RX        |<---- Manual control TX (when `MANUAL_SOFTWARE_SERIAL` is set)
   |      [11] Manual TX        |----> Manual control RX (when `MANUAL_SOFTWARE_SERIAL` is set)
   +----------------------------+
```

## Configuration
All main settings are in the `const.h` file:
- Compass pins and parameters: `COMPASS_SCL_PIN`, `COMPASS_SDA_PIN`, `COMPASS_DIRECTION`
- Servo pins and parameters: `ANTENNA_MAIN_PIN`, `ANTENNA_MAIN_MIN_FREQURENCE`, `ANTENNA_MAIN_MAX_FREQURENCE`, `ANTENNA_MAIN_SPEED`, `ANTENNA_MAIN_ACCEL`, `ANTENNA_SECOND_PIN`, `ANTENNA_SECOND_MIN_FREQURENCE`, `ANTENNA_SECOND_MAX_FREQURENCE`, `ANTENNA_SECOND_SPEED`, `ANTENNA_SECOND_ACCEL`, `ANTENNA_SECOND_MIN_ANGLE`, `ANTENNA_SECOND_MAX_ANGLE`
- MAVLink parameters: `MAVLINK_PORT`, `MAVLINK_RX_PIN`, `MAVLINK_TX_PIN`, `MAVLINK_BAUD`
- Manual control mode: Uncomment the required macro in `manual_control.h` (`MANUAL_HARDWARE_SERIAL`, `MANUAL_SOFTWARE_SERIAL`)

## Project Structure
- `antenna_stabilization.ino` - Main sketch, entry point
- `logger.*` - Logging class
- `compass.*` - Magnetic encoder AS5600 operations
- `manual_control.*` - Manual control
- `mav_control.*` - MAVLink message processing
- `antenna.*` - Servo motor control
- `logic.*` - Calculations and angle normalization
- `const.h` - All main constants and settings
- `servo.*` - Smooth servo motor control implementation

## Building and Uploading
1. Open the project in Arduino IDE or PlatformIO
2. Install required libraries:
   - ServoSmooth
   - MAVLink
   - AS5600
3. Connect the ESP32 to your computer
4. Select the appropriate port and board
5. Compile and upload the firmware

## Usage
- After uploading the firmware, the antenna will automatically position itself depending on the selected mode (auto/manual)
- Logging is available through the Serial Monitor

## Author
Bleyn (Vladislav Golovin)

## Contacts

[![Email](https://img.shields.io/badge/email-bleyn2017@gmail.com-blue?style=flat-square&logo=gmail)](mailto:bleyn2017@gmail.com)
[![Telegram](https://img.shields.io/badge/Telegram-BleynSpecnaz-2CA5E0?style=flat-square&logo=telegram)](https://t.me/BleynSpecnaz)
[![GitHub](https://img.shields.io/badge/GitHub-BleynChannel-181717?style=flat-square&logo=github)](https://github.com/BleynChannel)