/*
 * Logic namespace implementation
 *
 * Реализация функций для вычисления управляющего сигнала и нормализации углов для автостабилизации антенны.
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */

#include "logic.h"
#include <Arduino.h>

#define RAD_TO_DEG(x) (x * 57.2957795131) // x * 180 / PI
#define DEG_TO_RAD(x) (x * 0.01745329251) // x * PI / 180
#define CZ(x) (x >= -0.0000001f && x <= 0.0000001f ? 0.0f : x) // Clear Zero

Logic::Vector Logic::anglesToVector(Angles angles) {
    float az = DEG_TO_RAD(angles.azimuth);
    float el = DEG_TO_RAD(angles.elevation);

    return Logic::Vector {
        sin(az) * cos(el),
        cos(az) * cos(el),
        sin(el)
    };
}

Logic::Angles Logic::vectorToAngles(Vector vector) {
    // Определяем азимут (вращение по горизонту)
    float az = atan2(CZ(vector.x), CZ(vector.y)); //! Важно! Используется функция CZ, чтобы избежать проблему отрицательного нуля. Подробнее можно узнать здесь: https://stackoverflow.com/questions/47909048/what-will-be-atan2-output-for-both-x-and-y-as-0
    // Определяем угол возвышения (наклон вверх/вниз)
    float el = atan2(-vector.z, sqrt(vector.x*vector.x + vector.y*vector.y));

    return Logic::Angles {
        (uint16_t)(RAD_TO_DEG(az) + 360) % 360,
        (int16_t)RAD_TO_DEG(el)
    };
}

Logic::Vector Logic::globalToLocal(Rotate rotateLocal, Vector targetGlobal) {
    float cr = cos(rotateLocal.roll), sr = sin(rotateLocal.roll);
    float cp = cos(rotateLocal.pitch), sp = sin(rotateLocal.pitch);
    float cy = cos(rotateLocal.yaw), sy = sin(rotateLocal.yaw);
    float Xg = targetGlobal.x;
    float Yg = targetGlobal.y;
    float Zg = targetGlobal.z;

    /**
     * Преобразование глобального вектора в локальный по локальным координатам (roll, pitch, yaw)
     * 
     * ? Как это работает?
     * * Данное выражение является краткое описание формулы матричного вращения глобального вектора
     * * Оригинальная формула звучит так:
     * 
     * ! target_local=RT×target_global - перевод глобального вектора в локальный, где
     * 
     * RT - транспонированная матрица R
     * 
     * ! R=Rz(ϕ)×Ry(ψ)×Rx(θ) - общая матрица поворота, где
     * 
     * ϕ — Roll (вращение вокруг X)
     * θ — Pitch (вращение вокруг Y)
     * ψ — Yaw (вращение вокруг Z)
     * 
     *          [ 1     0      0    ]
     * Rx(θ) =  [ 0  cos(θ) -sin(θ) ]
     *          [ 0  sin(θ)  cos(θ) ]
     * 
     *          [ cos(ψ)  0  sin(ψ) ]
     * Ry(ψ) =  [ 0       1     0   ]
     *          [ -sin(ψ) 0  cos(ψ) ]
     * 
     *          [ cos(ϕ)  -sin(ϕ)  0 ]
     * Rz(ϕ) =  [ sin(ϕ)   cos(ϕ)  0 ]
     *          [   0        0     1 ]
    */

    float Xl = cr*cy*Xg + sr*cy*Yg - sy*Zg;
    float Yl = -sr*cp*Xg + cr*sy*sp*Xg + cr*cp*Yg + sr*sy*sp*Yg + cy*sp*Zg;
    float Zl = sr*sp*Xg + cr*sy*cp*Xg - cr*sp*Yg + sr*sy*cp*Yg + cy*cp*Zg;

    return Logic::Vector {Xl, Yl, Zl};
}