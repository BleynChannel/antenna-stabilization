/*
 * Antenna class header
 *
 * Описание: Класс для управления антенной (серво-приводом).
 * Предоставляет методы инициализации, вращения и преобразования углов/импульсов.
 *
 * Автор: Bleyn (Vladislav Golovin)
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
        float accel;
        uint16_t target;
    };

    struct SecondAntennaSetting {
        uint8_t pin;
        uint16_t min;
        uint16_t max;
        uint16_t speed;
        float accel;
        uint16_t target;
        int16_t minAngle;
        int16_t maxAngle;
    };
public:
    Antenna();
    void init(AntennaSetting mainSetting, SecondAntennaSetting secondSetting, Compass* compass);
    void tick();
    void rotate(uint16_t azimuth, int16_t elevation);
private:
    Compass *compass;
    ServoController mainServo;
    ServoSmooth secondServo;
    int16_t secondMinAngle;
    int16_t secondMaxAngle;
};

#endif