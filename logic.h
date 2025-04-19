#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>

namespace Logic {
    // Нормализация угла: 0..360 -> -180..180, где 0 - север
    // int16_t normalizeAngle(uint16_t angle);

    int16_t calculate(int16_t angle, int16_t carAngle, int16_t customAngle);
}

#endif