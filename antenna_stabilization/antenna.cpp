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

void Antenna::init(AntennaSetting mainSetting, SecondAntennaSetting secondSetting, Compass* compass) {
    // Инициализация серво и установка текущего положения
    mainServo.setSpeed(mainSetting.speed); // ограничить скорость
    mainServo.setAccel(mainSetting.accel); // установить ускорение (разгон и торможение)
    mainServo.attach(mainSetting.pin, mainSetting.min, mainSetting.max);
    mainServo.setCurrentAngle(mainSetting.target);
    mainServo.setAutoDetach(false);
    // mainServo.smoothStart();

    secondServo.setSpeed(secondSetting.speed); // ограничить скорость
    secondServo.setAccel(secondSetting.accel); // установить ускорение (разгон и торможение)
    secondServo.attach(secondSetting.pin, secondSetting.min, secondSetting.max, secondSetting.target);
    secondServo.setCurrentDeg(90); // Жёстко устанавливаем начальный угол в середине поворота
    secondServo.setMaxAngle(180); // Жёстко ограничиваем угол
    secondMinAngle = secondSetting.minAngle;
    secondMaxAngle = secondSetting.maxAngle;
    // secondServo.smoothStart();

    // Инициализация компаса
    this->compass = compass;
}

void Antenna::tick() {
    // Установка угла, полученного из магнетометра
    mainServo.setCurrentAngle(compass->getAngle());

    // Обновление состояния серво
    mainServo.tick();
    secondServo.tick();
}

void Antenna::rotate(uint16_t mainAngle, int16_t secondAngle) {
    // Вращение антенны
    mainServo.rotate(mainAngle);
    secondServo.setTargetDeg(constrain(secondAngle, secondMinAngle, secondMaxAngle) + 90);

    logger.print("Target [main]: ", mainServo.getTarget(), "; ");
    logger.print("Current [main]: ", mainServo.getCurrent(), "; ");
    logger.print("Target [second]: ", secondServo.getTargetDeg(), "; ");
    logger.print("Current [second]: ", secondServo.getCurrentDeg(), "; ");
}
