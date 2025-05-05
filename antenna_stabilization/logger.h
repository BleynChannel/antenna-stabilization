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
#include <iostream>
#include <type_traits>

#define DEBUG

class Logger {
public:
    Logger();

    // Инициализация Serial-порта для логирования
    void init(uint32_t baud);
    
    // Универсальная печать (без переноса строки)
    template <typename... Args>
    void print(Args... args) {
        #ifdef DEBUG
            (printImpl(args), ...);
        #endif
    }

    // Универсальная печать (с переносом строки)
    template <typename... Args>
    void println(Args... args) {
        #ifdef DEBUG
            (printImpl(args), ...);
            Serial.println();
        #endif
    }

    // Печать информационного сообщения
    template <typename... Args>
    void info(Args... args) {
        #ifdef DEBUG
            println("[INFO] ", args...);
        #endif
    }

    // Печать предупреждения
    template <typename... Args>
    void warning(Args... args) {
        #ifdef DEBUG
            println("[WARNING] ", args...);
        #endif
    }

    // Печать ошибки
    template <typename... Args>
    void error(Args... args) {
        #ifdef DEBUG
            println("[ERROR] ", args...);
        #endif
    }
private:
    void printImpl(float value) {
        #ifdef DEBUG
            Serial.print(value, 3);
        #endif
    }

    template <typename T>
    typename std::enable_if<!std::is_same<T, float>::value>::type
    printImpl(const T& value) {
        #ifdef DEBUG
            Serial.print(value);
        #endif
    }
};

extern Logger logger;

#endif