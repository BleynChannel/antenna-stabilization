/*
 * Logic namespace header
 *
 * Описание: Функции для вычисления управляющего сигнала и нормализации углов для автостабилизации антенны.
 * Предоставляет метод calculate для вычисления угла поворота серво на основе показаний компаса, угла транспортного средства и внешнего задаваемого угла.
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */

#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>

namespace Logic {
    // Нормализация угла: 0..360 -> -180..180, где 0 - север
    // int16_t normalizeAngle(uint16_t angle);

    uint16_t calculate(uint16_t angle, uint16_t customAngle);
}

#endif