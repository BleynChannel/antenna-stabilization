/*
 * ManualControl class implementation
 * Автор: BleynChannel (Golovin Vladislav)
 */
#include "manual_control.h"
#include "const.h"
#include "logger.h"

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
ManualControl::ManualControl(uint8_t pin) : pin(pin) {}
// Инициализация управления с помощью потенциометра
void ManualControl::init() {}
#endif

// Получение угла поворота
uint16_t ManualControl::getAngle() {
    #if defined(MANUAL_SOFTWARE_SERIAL)
    // Получение угла из программного управления
    if (serial.available()) angle = (uint16_t)serial.parseInt();
    return angle;
    #elif defined(MANUAL_HARDWARE_SERIAL)
    // Получение угла из аппаратного управления
    if (Serial.available()) angle = (uint16_t)Serial.parseInt();
    return angle;
    #elif defined(MANUAL_POT)
    // Получение угла из потенциометра
    return (uint16_t)map(analogRead(pin), MANUAL_MIN_FREQURENCE, MANUAL_MAX_FREQURENCE, 0, 360); //TODO: Вывести мин/макс в отдельный параметр (избавиться от const.h)
    #endif

    // set_angle = (received_angle + 180) % 360;     // Инвертирование угла}
}