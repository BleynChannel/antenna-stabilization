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

// #define MANUAL_SOFTWARE_SERIAL
#define MANUAL_HARDWARE_SERIAL

#define DEBUG

class ManualControl {
public:
    struct Data {
        float x = 0.0;
        float y = 1.0;
        float z = 0.0;
    };
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
    #endif
    
    Data getData();
private:
    Data data;
    String inString = "";
    int index = 0;

    #if defined(MANUAL_SOFTWARE_SERIAL)
    SoftwareSerial serial;
    #endif
};

#endif