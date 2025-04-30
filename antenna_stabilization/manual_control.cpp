/*
 * ManualControl class implementation
 * Автор: BleynChannel (Golovin Vladislav)
 */
#include "manual_control.h"

#if defined(MANUAL_SOFTWARE_SERIAL)
// Конструктор для программного управления
ManualControl::ManualControl(uint8_t rx, uint8_t tx) : serial(rx, tx) {}
// Инициализация программного управления
void ManualControl::init(uint32_t baud) { serial.begin(baud); }
#elif defined(MANUAL_HARDWARE_SERIAL)
// Конструктор для аппаратного управления с отладкой
ManualControl::ManualControl() {}

// Инициализация аппаратного управления с отладкой
#if !defined(DEBUG)
void ManualControl::init(uint32_t baud) { Serial.begin(baud); }
#else
void ManualControl::init() {}
#endif
#elif defined(MANUAL_POT)
// Конструктор для управления с помощью потенциометра
ManualControl::ManualControl(uint8_t pin, int32_t min, int32_t max) : pin(pin), min(min), max(max) {}
// Инициализация управления с помощью потенциометра
void ManualControl::init() {}
#endif

// Получение угла поворота
ManualControl::Data ManualControl::getData() {
    #if defined(MANUAL_SOFTWARE_SERIAL)
    while (serial.available()) {
        int inChar = serial.read();
    #elif defined(MANUAL_HARDWARE_SERIAL)
    while (Serial.available()) {
        int inChar = Serial.read();
    #endif

        if (inChar == ' ' || inChar == '\n') {
            switch (index++) {
                case 0:
                    data.mainAngle = (uint16_t)inString.toInt();
                    break;
                case 1:
                    data.secondAngle = (int16_t)inString.toInt();
                    break;
            }
            inString = "";
            if (inChar == '\n') index = 0;
        } else {
            inString += (char)inChar;
        }
    }

    return data;
}