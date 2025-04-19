/*
 * Compass class header
 *
 * Описание: Класс-обёртка для работы с датчиком положения AS5600.
 * Предоставляет методы инициализации, получения интенсивности сигнала и угла.
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */
#ifndef COMPASS_H
#define COMPASS_H

#include "AS5600.h"
#include <stdint.h>

class Compass {
public:
    Compass();
    void init(uint8_t dir = 0);
    void init(uint8_t scl, uint8_t sda, uint8_t dir = 0);
    uint16_t getIntensity();
    uint16_t getAngle();
private:
    void __init(uint8_t dir);
private:
    AS5600 compass;
};

#endif