#ifndef _ServoSmooth_h
#define _ServoSmooth_h
#include <Arduino.h>
#if defined(ESP32)
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif
#include <smoothUtil.h>

#define _SERVO_DEADZONE 4      // мёртвая зона (по микросекундам); По умолчанию - 10
#define _SERVO_DEADZONE_SP 1    // мёртвая зона (по скорости); По умолчанию - 3

class ServoController : public Smooth {
public:
    ServoController(int maxAngle = 180);
    using Smooth::attach;
    void attach(int pin);    
    void detach();
    void sendToDriver(uint16_t val);    
    Servo _servo;            
private:
};
#endif