/*
 * Antenna class implementation
 *
 * Реализация методов класса Antenna для управления антенной (серво-приводом).
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */
#include "antenna.h"
#include "compass.h"
#include "logic.h"
#include "servo.h"
#include "const.h"
#include "logger.h"

//TODO: Вынести функцию выше (избавиться от const.h)
Antenna::AntennaSetting Antenna::makeDefaultAntenna() {
    return Antenna::AntennaSetting(ANTENNA_MAIN_PIN, ANTENNA_MAIN_MIN_FREQURENCE, ANTENNA_MAIN_MAX_FREQURENCE, ANTENNA_MAIN_SPEED, ANTENNA_MAIN_ACCEL, 0);
}

Antenna::AntennaSetting Antenna::makeAntenna(uint8_t pin, uint16_t min, uint16_t max, uint16_t speed, uint16_t accel, uint16_t target) {
    return Antenna::AntennaSetting(pin, min, max, speed, accel, target);
}

Antenna::Antenna() {}

void Antenna::init(AntennaSetting mainSetting, AntennaSetting secondSetting, Compass* compass) {
    // Инициализация серво и установка текущего положения
    mainServo.setSpeed(mainSetting.speed); // ограничить скорость
    mainServo.setAccel(mainSetting.accel); // установить ускорение (разгон и торможение)
    mainServo.attach(mainSetting.pin, mainSetting.min, mainSetting.max);
    mainServo.setCurrentAngle(mainSetting.target);
    // mainServo.smoothStart();

    // Инициализация компаса
    this->compass = compass;
}

void Antenna::tick() {
    // Установка угла, полученного из магнетометра
    mainServo.setCurrentAngle(compass->getAngle());

    // Обновление состояния серво
    mainServo.tick();
    // secondServo.tick();
}

void Antenna::rotate(uint16_t mainAngle, uint16_t secondAngle) {
    // Вращение антенны
    mainServo.rotate(mainAngle);

    logger.print("Target: ", mainServo.getTarget(), "; ");
    logger.print("Current: ", mainServo.getCurrent(), "; ");
}
