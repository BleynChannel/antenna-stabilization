/*
 * Compass class implementation
 *
 * Реализация методов класса Compass для работы с датчиком AS5600.
 *
 * Автор: Bleyn (Vladislav Golovin)
 */
#include "compass.h"
#include "logger.h"
#include "Wire.h"

// Конструктор: инициализация объекта AS5600 по адресу
Compass::Compass() {}

void Compass::init(uint8_t dir) {
    logger.info("Compass init...");
    Wire.begin();
    __init(dir);
}

// Инициализация датчика
void Compass::init(uint8_t scl, uint8_t sda, uint8_t dir) {
    logger.info("Compass init...");
    Wire.begin(scl, sda);
    __init(dir);
}

void Compass::__init(uint8_t dir) {
    //? Дополнительный блок
    compass.begin(dir);
    compass.setDirection(AS5600_CLOCK_WISE);

    if (compass.isConnected()) {
        logger.info("Compass initialized");
    } else {
        logger.error("Compass not connected. Please check the connection");
    }
}

// Получение интенсивности сигнала. 0..4095
uint16_t Compass::getIntensity() {
    return compass.rawAngle();
}

// Получение угла. 0..360
uint16_t Compass::getAngle() {
    uint16_t angle = compass.rawAngle() * AS5600_RAW_TO_DEGREES;
    return angle;
}
