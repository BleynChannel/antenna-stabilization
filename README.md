# Antenna Turn MCU Firmware

## Описание

Этот проект представляет собой прошивку для микроконтроллера (ESP32), управляющего антенной с возможностью автоматического и ручного позиционирования. Система реализует автостабилизацию антенны под определённый динамический угол — антенна автоматически отслеживает и удерживает заданное направление, используя данные с компаса и/или автопилота (ArduPilot).

Система поддерживает работу с компасом (AS5600), ручное управление (потенциометр/Serial), а также интеграцию с MAVLink для получения параметров от внешних устройств (например, автопилотов на базе ArduPilot).

## Основные возможности
- Автоматическая стабилизация антенны под динамический угол (автонаведение)
- Управление антенной через серво-приводы с плавным позиционированием
- Получение угла с магнитного энкодера (AS5600)
- Ручное управление положением антенны (потенциометр, аппаратный или программный Serial)
- Получение и обработка данных по MAVLink (VFR_HUD, Altitude)
- Гибкая архитектура: отдельные классы для логгирования, логики, MAVLink, компаса и ручного управления
- Поддержка расширения под дополнительные типы датчиков и протоколов

## Подключение
- **MCU:** ESP32 (или совместимый)
- **Компас:** AS5600 (I2C)
- **Серво-приводы:** Подключение к пинам, заданным в `const.h`
- **Ручное управление:** Потенциометр или Serial (см. конфигурацию)
- **MAVLink:** Подключение к автопилоту (например, SpeedyBee F405 V4, Matek, Pixhawk и др. на ArduPilot) напрямую к RX/TX портам

## Схема подключения (распиновка)

| Назначение         | ESP32 GPIO | Описание/Примечание         |
|--------------------|------------|-----------------------------|
| Servo PWM          |    34      | ANTENNA_MAIN_PIN            |
| Servo Min Pulse    |   1435     | ANTENNA_MAIN_MIN_FREQURENCE |
| Servo Max Pulse    |   1575     | ANTENNA_MAIN_MAX_FREQURENCE |
| Compass SCL        |    22      | COMPASS_SCL_PIN (I2C SCL)   |
| Compass SDA        |    21      | COMPASS_SDA_PIN (I2C SDA)   |
| Compass Direction  |     0      | COMPASS_DIRECTION           |
| Manual Pot         |    12      | MANUAL_PIN_PORT (ADC)       |
| MAVLink RX         |    16      | MAVLINK_RX_PIN              |
| MAVLink TX         |    17      | MAVLINK_TX_PIN              |

> Все значения пинов и параметров можно изменить в файле `const.h`.
> 
> **MAVLink RX/TX**: Подключение осуществляется напрямую к UART-портам полетного контроллера (например, SpeedyBee F405 V4, Matek, Pixhawk и др.), поддерживающего ArduPilot.

```
ESP32
   +-------------------------+
   |      [34] Servo PWM     |----> Серво-привод
   |      [22] SCL           |----> AS5600 SCL
   |      [21] SDA           |----> AS5600 SDA
   |      [12] Pot           |----> Потенциометр (ручное управление)
   |      [16] RX            |<---- TX автопилота (ArduPilot)
   |      [17] TX            |----> RX автопилота (ArduPilot)
   +-------------------------+
```

## Конфигурация
Все основные настройки находятся в файле `const.h`:
- Пины и параметры компаса: `COMPASS_SCL_PIN`, `COMPASS_SDA_PIN`, `COMPASS_DIRECTION`
- Пины и параметры серво: `ANTENNA_MAIN_PIN`, `ANTENNA_MAIN_MIN_FREQURENCE`, `ANTENNA_MAIN_MAX_FREQURENCE`, `ANTENNA_MAIN_SPEED`, `ANTENNA_MAIN_ACCEL`
- Параметры MAVLink: `MAVLINK_PORT`, `MAVLINK_RX_PIN`, `MAVLINK_TX_PIN`, `MAVLINK_BAUD`
- Режим ручного управления: раскомментируйте нужный макрос в `manual_control.h` (`MANUAL_POT`, `MANUAL_SOFTWARE_SERIAL`, `MANUAL_HARDWARE_SERIAL`)

## Структура проекта
- `antenna_turn.ino` — основной скетч, точка входа
- `logger.*` — класс для логирования
- `compass.*` — работа с магнитным энкодером AS5600
- `manual_control.*` — ручное управление
- `mav_control.*` — обработка MAVLink-сообщений
- `antenna.*` — управление сервоприводом
- `logic.*` — вычисления и нормализация углов
- `const.h` — все основные константы и настройки

## Сборка и загрузка
1. Откройте проект в Arduino IDE или PlatformIO
2. Установите необходимые библиотеки:
   - ServoSmooth
   - MAVLink (если требуется)
   - AS5600
3. Подключите ESP32 к компьютеру
4. Выберите нужный порт и плату
5. Скомпилируйте и загрузите прошивку

## Использование
- После загрузки прошивки антенна будет автоматически позиционироваться в зависимости от выбранного режима (авто/ручной)
- Логирование доступно через Serial Monitor

## Автор
BleynChannel (Golovin Vladislav)

## Контакты

[![Email](https://img.shields.io/badge/email-bleyn2017@gmail.com-blue?style=flat-square&logo=gmail)](mailto:bleyn2017@gmail.com)
[![Telegram](https://img.shields.io/badge/Telegram-BleynSpecnaz-2CA5E0?style=flat-square&logo=telegram)](https://t.me/BleynSpecnaz)
[![GitHub](https://img.shields.io/badge/GitHub-BleynChannel-181717?style=flat-square&logo=github)](https://github.com/BleynChannel)