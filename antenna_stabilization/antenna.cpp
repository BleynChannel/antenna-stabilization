/*
 * Antenna class implementation
 *
 * Реализация методов класса Antenna для управления антенной (серво-приводом).
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */
#include "antenna.h"
#include "compass.h"
#include "logger.h"

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
