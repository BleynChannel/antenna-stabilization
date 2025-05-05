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
    float az = atan2(vector.x, vector.y);
    // Определяем угол возвышения (наклон вверх/вниз)
    float el = atan2(-vector.z, sqrt(vector.x*vector.x + vector.y*vector.y));

    return Logic::Angles {
        (uint16_t)RAD_TO_DEG(az) % 360,
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
     *          [ cos(ϕ)   sin(ϕ)  0 ]
     * Rz(ϕ) =  [ -sin(ϕ)  cos(ϕ)  0 ]
     *          [   0        0     1 ]
    */
    float Xl =  cp * cy * Xg + cp * sy * Yg - sp * Zg;
    float Yl = (sr * sp * cy - cr * sy) * Xg + (sr * sp * sy + cr * cy) * Yg + cp * sr * Zg;
    float Zl = (cr * sp * cy + sr * sy) * Xg + (cr * sp * sy - sr * cy) * Yg + cp * cr * Zg;

    return Logic::Vector {Xl, Yl, Zl};
}