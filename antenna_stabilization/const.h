/*
 * const.h header
 *
 * Описание: Константы для работы программы.
 *
 * Автор: Bleyn (Vladislav Golovin)
 */

#ifndef CONST_H
#define CONST_H

// Таймер основного цикла (мс)
#define TIMER_INTERVAL 40

// Параметры логгера
#define LOGGER_BAUD 115200

// Настройки компаса (I2C)
// #define COMPASS_ADDRESS 0x36 // Адрес датчика (если используется)
#define COMPASS_SCL_PIN 22      // Линия SCL
#define COMPASS_SDA_PIN 21      // Линия SDA
#define COMPASS_DIRECTION 0     // Направление калибровки

// Ручное управление (Serial)
#define MANUAL_RX_PIN 10        // Прием
#define MANUAL_TX_PIN 11        // Передача
#define MANUAL_BAUD 115200      // Скорость передачи

// MAVLink (Serial)
#define MAVLINK_PORT 2          // Номер порта
#define MAVLINK_RX_PIN 16       // Прием
#define MAVLINK_TX_PIN 17       // Передача
#define MAVLINK_BAUD 115200     // Скорость передачи

// Основная антенна (сервопривод)
#define ANTENNA_MAIN_PIN 27              // Пин управления
#define ANTENNA_MAIN_MIN_FREQURENCE 1470 // Минимальная частота
#define ANTENNA_MAIN_MAX_FREQURENCE 1540 // Максимальная частота
#define ANTENNA_MAIN_SPEED 90            // Скорость движения
#define ANTENNA_MAIN_ACCEL 0.3           // Ускорение
#define ANTENNA_MAIN_TARGET 0            // Целевая позиция

// Вспомогательная антенна (сервопривод)
#define ANTENNA_SECOND_PIN 26              // Пин управления
#define ANTENNA_SECOND_MIN_FREQURENCE 500  // Минимальная частота
#define ANTENNA_SECOND_MAX_FREQURENCE 2500 // Максимальная частота
#define ANTENNA_SECOND_SPEED 90            // Скорость движения
#define ANTENNA_SECOND_ACCEL 0.2           // Ускорение
#define ANTENNA_SECOND_TARGET 0            // Целевая позиция
#define ANTENNA_SECOND_MIN_ANGLE -20       // Минимальный угол
#define ANTENNA_SECOND_MAX_ANGLE 45        // Максимальный угол

//? Нужна ли начальная точка в среднем положение?
// #define START_FREQURENCE (SERVO_MAX_FREQURENCE + SERVO_MIN_FREQURENCE) / 2

#endif