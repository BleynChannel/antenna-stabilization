#include "servo.h"

ServoController::ServoController() {
    _maxAngle = 360;
    SS_DEADZONE = _SERVO_DEADZONE;
    SS_DEADZONE_SP = _SERVO_DEADZONE_SP;
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