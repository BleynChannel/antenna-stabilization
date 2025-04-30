#include "servo.h"

#define INVERT -1 // Необходимо для корректного движения сервомоторчика

ServoController::ServoController() {
    _maxAngle = 360; // Полный угол поворота
    SS_DEADZONE = _SERVO_DEADZONE;
    SS_DEADZONE_SP = _SERVO_DEADZONE_SP;
    SS_LIMIT = _SERVO_LIMIT;
}

void ServoController::sendToDriver(uint16_t val) {
    _servo.writeMicroseconds(val);
}

void ServoController::attach(int pin) {
    _pin = pin;
    _servo.attach(_pin);
}

void ServoController::detach() {
    _servo.detach();
}

void ServoController::setCurrentAngle(uint16_t angle) {
    _currentAngle = constrain(angle, 0, _maxAngle);
}

// Реализация поворота сервомоторчика по кратчайшему углу с плавным движением
void ServoController::rotate(uint16_t angle) {
    // Ограничиваем угл поворота
    angle = constrain(angle, 0, _maxAngle);
    
    // Вычисляем разницу между текущим углом и целевым. Нормализуем угл. (-180..180)
    int16_t diff = ((_currentAngle - angle) + 540) % 360 - 180;

    // Вычисляем значение после мёртвой зоны
    float value = max(0, abs(diff) - SS_DEADZONE) / (SS_LIMIT - SS_DEADZONE);

    // Применяем знак и ограничение по максимуму/минимуму
    value = constrain(((diff > 0) - (diff < 0)) * value, -1.0, 1.0) * INVERT;

    // Преобразовываем разницу углов поворота в интенсивность и задаем целевое положение
    _servoTargetPos = uint16_t((value - -1.0) * (_max - _min) / (1.0 - -1.0) + _min);
}