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
            #if defined(MANUAL_VECTOR)
            switch (index++) {
                case 0:
                    data.x = inString.toFloat();
                    break;
                case 1:
                    data.y = inString.toFloat();
                    break;
                case 2:
                    data.z = inString.toFloat();
                    break;
            }
            #elif defined(MANUAL_ANGLES)
            switch (index++) {
                case 0:
                    data.azimuth = (uint16_t)inString.toInt() % 360;
                    break;
                case 1:
                    data.elevation = (int16_t)inString.toInt() % 90;
                    break;
            }
            #endif

            inString = "";
            if (inChar == '\n') index = 0;
        } else {
            inString += (char)inChar;
        }
    }

    return data;
}