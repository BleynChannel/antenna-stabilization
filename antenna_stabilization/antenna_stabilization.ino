/*
 ============================================================================
  Project     : Antenna Auto-Positioning System
  Description : Управление антенной с автопозиционированием на базе ESP32.
                Используются датчики положения (AS5600) и компас (QMC5883L).
                Проект интегрируется с MissionPlanner.
 
  Developer   : Bleyn (Vladislav Golovin)
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
ManualControl manualControl;
#elif defined(MANUAL_HARDWARE_SERIAL) && defined(DEBUG)
ManualControl manualControl;
#endif

MAVControl mavControl(MAVLINK_PORT);
Antenna antenna;

//* Данные
Logic::Vector targetVector;
#if defined(MANUAL_ANGLES)
Logic::Angles targetAngles;
#endif

Logic::Rotate carRotate;
Logic::Angles antennaAngles;

void setup() {
  delay(3000);  // Задержка 3 секунды при запуске
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
  // Ручной ввод
  ManualControl::Data manualData = manualControl.getData();
  #if defined(MANUAL_VECTOR)
  logger.print("Manual [X]: ", manualData.x, "; ");
  logger.print("Manual [Y]: ", manualData.y, "; ");
  logger.print("Manual [Z]: ", manualData.z, "; ");
  #elif defined(MANUAL_ANGLES)
  logger.print("Manual [Azimuth]: ", manualData.azimuth, "; ");
  logger.print("Manual [Elevation]: ", manualData.elevation, "; ");
  #endif

  // MAVLink
  mavlink_attitude_t carAttitude = mavControl.getAttitude();
  logger.print("Car MAV [Roll]: ", carAttitude.roll, "; ");
  logger.print("Car MAV [Pitch]: ", carAttitude.pitch, "; ");
  logger.print("Car MAV [Yaw]: ", carAttitude.yaw, "; ");

  //! Важно! Замена roll <-> yaw сделано для того, чтобы компенсировать особенность ArduPilot
  carRotate = Logic::Rotate {
    carAttitude.yaw,
    carAttitude.pitch,
    carAttitude.roll
  };

  #if defined(MANUAL_VECTOR)
  targetVector = Logic::Vector {
    manualData.x,
    manualData.y,
    manualData.z
  };
  #elif defined(MANUAL_ANGLES)
  targetAngles = Logic::Angles {
    manualData.azimuth,
    manualData.elevation
  };
  #endif
}

void calculate() {
  // Вычисляем поворот антенны
  #if defined(MANUAL_ANGLES)
  targetVector = Logic::anglesToVector(targetAngles);
  #endif

  Logic::Vector targetLocalVector = Logic::globalToLocal(carRotate, targetVector);
  logger.print("Antenna [X]: ", targetLocalVector.x, "; ");
  logger.print("Antenna [Y]: ", targetLocalVector.y, "; ");
  logger.print("Antenna [Z]: ", targetLocalVector.z, "; ");
  
  // Получаем углы поворота из локального вектора
  antennaAngles = Logic::vectorToAngles(targetLocalVector);
  logger.print("Antenna [Azimuth]: ", antennaAngles.azimuth, "; ");
  logger.print("Antenna [Elevation]: ", antennaAngles.elevation, "; ");
}

void applyCalculate() {
  antenna.rotate(antennaAngles.azimuth, antennaAngles.elevation);
}