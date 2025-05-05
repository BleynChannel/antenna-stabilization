/*
 TODO: Изменить описание
 * Logic namespace header
 *
 * Описание: Функции для вычисления управляющего сигнала и нормализации углов для автостабилизации антенны.
 * Предоставляет метод globalToLocal для вычисления угла поворота серво на основе показаний компаса, угла транспортного средства и внешнего задаваемого угла.
 *
 * Автор: Bleyn (Vladislav Golovin)
 */

#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>

namespace Logic {
    struct Vector {
        float x = 0.0, y = 0.0, z = 0.0;
    };

    struct Rotate {
        float roll = 0.0, pitch = 0.0, yaw = 0.0;
    };

    struct Angles {
        uint16_t azimuth = 0;
        int16_t elevation = 0;
    };

    // Перевод азимута и угла возвышения (в градусах) в направленный вектор
    Vector anglesToVector(Angles angles);
    
    // Перевод вектора в углы поворота
    Angles vectorToAngles(Vector vector);

    // Вычисление локального направления из глобального
    Vector globalToLocal(Rotate rotateLocal, Vector targetGlobal);
}

#endif