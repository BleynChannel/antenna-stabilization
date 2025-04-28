/*
 * Antenna class implementation
 *
 * Реализация методов класса Antenna для управления антенной (серво-приводом).
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */
#include "antenna.h"
#include "logic.h"
#include "servo.h"
#include "const.h"
#include "logger.h"

// TODO: Временные решения

// Интенсивность в угл
uint16_t Antenna::int2Angle(uint16_t intensity) {
    return map(intensity, ANTENNA_MAIN_MIN_FREQURENCE, ANTENNA_MAIN_MAX_FREQURENCE, 0, 360);
}

// Угл в интенсивность
uint16_t Antenna::angle2Int(uint16_t angle) {
    return map(angle, 0, 360, ANTENNA_MAIN_MIN_FREQURENCE, ANTENNA_MAIN_MAX_FREQURENCE);
}

//! Основные методы

Antenna::AntennaSetting Antenna::makeDefaultAntenna() {
    return Antenna::AntennaSetting(ANTENNA_MAIN_PIN, ANTENNA_MAIN_MIN_FREQURENCE, ANTENNA_MAIN_MAX_FREQURENCE, ANTENNA_MAIN_SPEED, ANTENNA_MAIN_ACCEL, 0);
}

Antenna::AntennaSetting Antenna::makeAntenna(uint8_t pin, uint16_t min, uint16_t max, uint16_t speed, uint16_t accel, uint16_t current) {
    return Antenna::AntennaSetting(pin, min, max, speed, accel, current);
}

Antenna::Antenna() {}

void Antenna::init(AntennaSetting mainSetting, AntennaSetting secondSetting) {
    // Инициализация серво и установка текущего положения
    mainServo.setSpeed(mainSetting.speed);   // ограничить скорость
    mainServo.setAccel(mainSetting.accel);  // установить ускорение (разгон и торможение)
    mainServo.attach(mainSetting.pin, mainSetting.min, mainSetting.max);  // 600 и 2400 - длины импульсов, при которых
    // серво поворачивается максимально в одну и другую сторону, зависят от самой серво
    // и обычно даже указываются продавцом. Мы их тут указываем для того, чтобы
    // метод setTargetDeg() корректно отрабатывал диапазон поворота сервы
    // mainServo.setCurrent(angle2Int(Logic::normalizeAngle(compass.getAngle())));
    // servo.smoothStart();

    // servo.writeMicroseconds(START_FREQURENCE);
}

void Antenna::tick() {
    // Обновление состояния серво
    mainServo.tick();
    // secondServo.tick();
}

//TODO: Позже заменить на _SERVO_DEADZONE
#define ANTENNA_MAIN_LIMIT 4 // Ставим порог для создания плавного движения
#define INVERT -1 // Необходимо для корректного движения сервомоторчика

void Antenna::rotate(int16_t diffAngle) {
    // Вычисление направления и интенсивности
    uint16_t intensity = map(
        constrain(diffAngle * INVERT, -ANTENNA_MAIN_LIMIT, ANTENNA_MAIN_LIMIT),
        -ANTENNA_MAIN_LIMIT, ANTENNA_MAIN_LIMIT,
        ANTENNA_MAIN_MIN_FREQURENCE, ANTENNA_MAIN_MAX_FREQURENCE);
    
    logger.print("Intensity: ", intensity, "; ");

    // Вращение антенны
    mainServo.setTarget(intensity);

    logger.print("Current: ", mainServo.getCurrent(), "; ");
}
