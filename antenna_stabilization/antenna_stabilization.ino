/*
 ============================================================================
  Project     : Antenna Auto-Positioning System
  Description : Управление антенной с автопозиционированием на базе ESP32.
                Используются датчики положения (AS5600) и компас (QMC5883L).
                Проект интегрируется с MissionPlanner.
 
  Developer   : BleynChannel (Golovin Vladislav)
  C++ Version : 17
  Hardware    : ESP32, AS5600, SpeedyBye F405 V4, QMC5883L
  Software    : Arduino IDE / PlatformIO, MissionPlanner
  License     : MIT

  Details     : 
      - Управление антенной по заданному и текущему углу.
      - Поддержка внешних датчиков и периферии.
      - Возможность расширения под другие платформы.
 ============================================================================
*/

#include "const.h"
#include "logger.h"
#include "compass.h"
#include "manual_control.h"
#include "mav_control.h"
#include "antenna.h"
#include "logic.h"

// Таймер
uint32_t timer;

//* Компоненты системы
Compass compass;

#if defined(MANUAL_SOFTWARE_SERIAL)
ManualControl manualControl(MANUAL_RX_PIN, MANUAL_TX_PIN);
#elif defined(MANUAL_HARDWARE_SERIAL) && defined(DEBUG)
ManualControl manualControl;
#elif defined(MANUAL_HARDWARE_SERIAL)
ManualControl manualControl(MANUAL_PIN_PORT);
#elif defined(MANUAL_POT)
ManualControl manualControl(MANUAL_PIN_PORT);
#endif

MAVControl mavControl(MAVLINK_PORT);
Antenna antenna;

//* Данные
uint16_t compassAngle; // Угл компаса. TODO: Убрать
uint16_t carAngle; // Угл транспорта
uint16_t customAngle; // Задаваемый угл
uint16_t diffAngle; // Разница углов

void setup() {
  logger.init(LOGGER_BAUD);
  // Инициализация компаса
  compass.init(COMPASS_SCL_PIN, COMPASS_SDA_PIN, COMPASS_DIRECTION);
  // Ручное управление
  
  // #if defined(MANUAL_SOFTWARE_SERIAL)
  // manualControl.init(MANUAL_BAUD);
  // #elif defined(MANUAL_HARDWARE_SERIAL) && !defined(DEBUG)
  // manualControl.init(MANUAL_BAUD);
  // #elif defined(MANUAL_POT) || (defined(MANUAL_HARDWARE_SERIAL) && defined(DEBUG))
  manualControl.init();
  // #endif

  // Инициализация MAVControl
  mavControl.init(MAVLINK_RX_PIN, MAVLINK_TX_PIN, MAVLINK_BAUD);
  // Инициализация антенны
  antenna.init(Antenna::makeDefaultAntenna(), Antenna::makeDefaultAntenna(), &compass);
}

void loop() {
  mavControl.tick();
  antenna.tick();
  if (millis() - timer >= TIMER_INTERVAL) {
    timer = millis();
    // Шаг 1: Получение внешних параметров
    getParametres();
    // Шаг 2: Расчёт
    calculate();
    // Шаг 3: Применение результата
    applyCalculate();
    logger.println();
  }
}

void getParametres() {
  // Угл компаса. 0..360
  compassAngle = compass.getAngle();
  logger.print("Magnetometr angle: ", compassAngle, "; ");
  // Угл транспорта. 0..360
  carAngle = mavControl.getVFRHud().heading;
  logger.print("Car angle: ", carAngle, "; ");
  // Ручной ввод. 0..360
  customAngle = manualControl.getAngle();
  logger.print("Custom angle: ", customAngle, "; ");
}

void calculate() {
  // Расчёт разницы углов. -180..180
  diffAngle = Logic::calculate(compassAngle, carAngle, customAngle);
  logger.print("Diff Angle: ", diffAngle, "; ");
}

void applyCalculate() {
  antenna.rotate(diffAngle, 0 /* Не используется */);
}