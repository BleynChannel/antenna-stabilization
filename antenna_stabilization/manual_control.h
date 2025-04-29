/*
 * ManualControl class header
 *
 * Описание: Класс для ручного управления антенной или устройством.
 * Предоставляет методы инициализации и получения значения угла.
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */
#ifndef MANUAL_CONTROL_H
#define MANUAL_CONTROL_H

#include <stdint.h>
#include <Arduino.h>
// #include <SoftwareSerial.h>

// Методы получения стороннего значения:
// 1. SoftwareSerial (#define MANUAL_SOFTWARE_SERIAL)
// 2. HardwareSerial (#define MANUAL_HARDWARE_SERIAL)
// 3. Потенциометр (#define MANUAL_POT)

// #define MANUAL_SOFTWARE_SERIAL
#define MANUAL_HARDWARE_SERIAL
// #define MANUAL_POT

#define DEBUG

class ManualControl {
public:
    #if defined(MANUAL_SOFTWARE_SERIAL)
    ManualControl(uint8_t rx, uint8_t tx);
    void init(uint32_t baud);
    #elif defined(MANUAL_HARDWARE_SERIAL)
    ManualControl();
    #if !defined(DEBUG)
    void init(uint32_t baud);
    #else
    void init();
    #endif
    #elif defined(MANUAL_POT)
    ManualControl(uint8_t pin, int32_t min, int32_t max);
    void init();
    #endif
    
    uint16_t getAngle();
private:
    #if defined(MANUAL_SOFTWARE_SERIAL) || defined(MANUAL_HARDWARE_SERIAL)
    uint16_t angle = 0;
    #endif

    #if defined(MANUAL_SOFTWARE_SERIAL)
    SoftwareSerial serial;
    #elif defined(MANUAL_POT)
    uint8_t pin;
    int32_t min;
    int32_t max;
    #endif
};

#endif