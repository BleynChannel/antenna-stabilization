/*
 * Logger class implementation
 *
 * Реализация методов класса Logger для логирования и отладки через Serial-порт.
 *
 * Автор: Bleyn (Vladislav Golovin)
 */
#include "logger.h"

Logger::Logger() {}

// Инициализация Serial-порта для логирования
void Logger::init(uint32_t baud) {
    #ifdef DEBUG
    Serial.begin(baud);
    #endif
}

Logger logger;