/*
 * Logic namespace implementation
 *
 * Реализация функций для вычисления управляющего сигнала и нормализации углов для автостабилизации антенны.
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */

#include "logic.h"

// Нормализация угла: 0..360 -> -180..180, где 0 - север
// int16_t Logic::normalizeAngle(uint16_t angle) {
//     return int16_t(angle + 180) % 360 - 180;
// }

uint16_t Logic::calculate(uint16_t angle, uint16_t customAngle) {
    /*  TODO: Необходимо получать интенсивность сигнала и направление поворота
        Но перед получением, необходимо получать разницу углов поворота, относительно нескольких параметров:
        1. Угл компоса (или по другому "угл серво")
        2. Угл транспорта
        3. Внешний задаваемый угл
    */

    // int16_t diffAngle = angle - customAngle;
    // diffAngle = (diffAngle + 540) % 360 - 180; // Нормализация угла
    // return diffAngle;

    return customAngle;
}
