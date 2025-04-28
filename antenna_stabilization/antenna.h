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

class Antenna {
public:
    struct AntennaSetting {
        uint8_t pin;
        uint16_t min;
        uint16_t max;
        uint16_t speed;
        uint16_t accel;
        uint16_t current;
    };
public:
    // TODO: Временные решения

    // Интенсивность в угл. 0..360
    static uint16_t int2Angle(uint16_t intensity);

    // Угл в интенсивность. MIN..MAX
    static uint16_t angle2Int(uint16_t angle);

    //! Основные методы

    // Создание настройки
    static AntennaSetting makeDefaultAntenna();
    static AntennaSetting makeAntenna(uint8_t pin, uint16_t min, uint16_t max, uint16_t speed, uint16_t accel, uint16_t current = 0);

    Antenna();
    void init(AntennaSetting mainSetting, AntennaSetting secondSetting);
    void tick();
    void rotate(int16_t diffAngle);
private:
    ServoController mainServo;
    // ServoSmooth secondServo;
};

#endif