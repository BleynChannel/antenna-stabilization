/*
 * Antenna class header
 *
 * Описание: Класс для управления антенной (серво-приводом).
 * Предоставляет методы инициализации, вращения и преобразования углов/импульсов.
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */
#ifndef ANTENNA_H
#define ANTENNA_H

#include "servo.h"
#include <ServoSmooth.h>
#include <stdint.h>

class Compass;

class Antenna {
public:
    struct AntennaSetting {
        uint8_t pin;
        uint16_t min;
        uint16_t max;
        uint16_t speed;
        uint16_t accel;
        uint16_t target;
    };
public:
    Antenna();
    void init(AntennaSetting mainSetting, AntennaSetting secondSetting, Compass* compass);
    void tick();
    void rotate(uint16_t mainAngle, uint16_t secondAngle); //TODO: Следующим шагом будет отправлять вектор направления антенны (x,y,z)
private:
    ServoController mainServo;
    // ServoSmooth secondServo;
    Compass *compass;
};

#endif