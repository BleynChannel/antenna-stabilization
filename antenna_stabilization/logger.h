/*
 * Logger class header
 *
 * Описание: Класс для организации логирования и отладки через Serial-порт.
 * Предоставляет методы для печати сообщений разного уровня (info, warning, error),
 * а также универсальные print/println с поддержкой произвольного количества аргументов.
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <stdint.h>

#define DEBUG

class Logger {
public:
    Logger();

    // Инициализация Serial-порта для логирования
    void init(uint32_t baud);
    
    // Универсальная печать (без переноса строки)
    template <typename... Args>
    void print(Args&&... args) {
        #ifdef DEBUG
            (Serial.print(args), ...);
        #endif
    }

    // Универсальная печать (с переносом строки)
    template <typename... Args>
    void println(Args&&... args) {
        #ifdef DEBUG
            (Serial.print(args), ...);
            Serial.println();
        #endif
    }

    // Печать информационного сообщения
    template <typename... Args>
    void info(Args&&... args) {
        #ifdef DEBUG
            println("[INFO] ", args...);
        #endif
    }

    // Печать предупреждения
    template <typename T>
    void warning(T value) {
        #ifdef DEBUG
            println("[WARNING] ", value);
        #endif
    }

    // Печать ошибки
    template <typename T>
    void error(T value) {
        #ifdef DEBUG
            println("[ERROR] ", value);
        #endif
    }
};

extern Logger logger;

#endif