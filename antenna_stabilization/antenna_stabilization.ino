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
#elif defined(MANUAL_HARDWARE_SERIAL) && !defined(DEBUG)
ManualControl manualControl(MANUAL_PIN_PORT);
#elif defined(MANUAL_HARDWARE_SERIAL) && defined(DEBUG)
ManualControl manualControl;
#endif

MAVControl mavControl(MAVLINK_PORT);
Antenna antenna;

//* Данные
uint16_t compassAngle; // Угл компаса. TODO: Убрать
uint16_t mainAngle; // Задаваемый угл для главного серво
int16_t secondAngle; // Задаваемый угл для вспомогательного серво
uint16_t diffAngle; // Разница углов

void setup() {
  logger.init(LOGGER_BAUD);
  // Инициализация компаса
  compass.init(COMPASS_SCL_PIN, COMPASS_SDA_PIN, COMPASS_DIRECTION);
  // Ручное управление
  
  #if defined(MANUAL_SOFTWARE_SERIAL)
  manualControl.init(MANUAL_BAUD);
  #elif defined(MANUAL_HARDWARE_SERIAL) && !defined(DEBUG)
  manualControl.init(MANUAL_BAUD);
  #elif defined(MANUAL_HARDWARE_SERIAL) && defined(DEBUG)
  manualControl.init();
  #endif

  // Инициализация MAVControl
  mavControl.init(MAVLINK_RX_PIN, MAVLINK_TX_PIN, MAVLINK_BAUD);
  // Инициализация антенны
  antenna.init(makeMainServo(), makeSecondServo(), &compass);
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

Antenna::AntennaSetting makeMainServo() {
  return Antenna::AntennaSetting(
    ANTENNA_MAIN_PIN, 
    ANTENNA_MAIN_MIN_FREQURENCE,
    ANTENNA_MAIN_MAX_FREQURENCE, 
    ANTENNA_MAIN_SPEED, 
    ANTENNA_MAIN_ACCEL,
    ANTENNA_MAIN_TARGET);
}

Antenna::SecondAntennaSetting makeSecondServo() {
  return Antenna::SecondAntennaSetting(
    ANTENNA_SECOND_PIN, 
    ANTENNA_SECOND_MIN_FREQURENCE, 
    ANTENNA_SECOND_MAX_FREQURENCE, 
    ANTENNA_SECOND_SPEED, 
    ANTENNA_SECOND_ACCEL, 
    ANTENNA_SECOND_TARGET, 
    ANTENNA_SECOND_MIN_ANGLE, 
    ANTENNA_SECOND_MAX_ANGLE);
}

void getParametres() {
  // Угл компаса. 0..360
  compassAngle = compass.getAngle();
  logger.print("Magnetometr angle: ", compassAngle, "; ");
  // Ручной ввод. 0..360
  mainAngle = manualControl.getData().mainAngle;
  logger.print("Main angle: ", mainAngle, "; ");
  // Ручной ввод. -180..180
  secondAngle = manualControl.getData().secondAngle;
  logger.print("Second angle: ", secondAngle, "; ");
}

void calculate() {
  // Расчёт разницы углов. -180..180
  diffAngle = Logic::calculate(compassAngle, mainAngle);
  logger.print("Diff Angle: ", diffAngle, "; ");
}

void applyCalculate() {
  antenna.rotate(mainAngle, secondAngle);
}