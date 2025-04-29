#ifndef _ServoSmooth_h
#define _ServoSmooth_h
#include <Arduino.h>
#if defined(ESP32)
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif
#include <smoothUtil.h>

#define _SERVO_DEADZONE 4       // мёртвая зона (по микросекундам); По умолчанию - 10
#define _SERVO_DEADZONE_SP 1    // мёртвая зона (по скорости); По умолчанию - 3
#define _SERVO_LIMIT 20         // ограничение углов для плавного поворота; По умолчанию - 20

class ServoController : public Smooth {
public:
    ServoController();
    using Smooth::attach;
    void attach(int pin);
    void detach();
    void sendToDriver(uint16_t val);
    
    void setCurrentAngle(uint16_t angle);
    void rotate(uint16_t angle);
private:
    Servo _servo;
    uint16_t _currentAngle; //? Данная переменная нужна только для установки в нее значения с магнетометра
protected:
    uint16_t SS_LIMIT;
};
#endif