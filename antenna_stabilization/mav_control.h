/*
 * MAVLink class header
 *
 * Описание: Класс-заглушка для работы с MAVLink или пользовательским протоколом.
 *
 * Автор: Bleyn (Vladislav Golovin)
 */
#ifndef MAV_CONTROL_H
#define MAV_CONTROL_H

#include <HardwareSerial.h>
#include <MAVLink.h>
#include <stdint.h>

class MAVControl {
public:
    MAVControl(uint8_t port);
    void init(uint8_t rx, uint8_t tx, uint32_t baud);
    void tick(uint32_t interval = 1000);
    
    mavlink_vfr_hud_t getVFRHud();
    mavlink_attitude_t getAttitude();
private:
    // MAVLink functions
    void requestData();
    void receiveData();
private:
    HardwareSerial serial;

    // Ticks variables
    uint64_t prevMillis;
    int numHbsPasados;

    // MAVLink Variables
    mavlink_vfr_hud_t vfrHud;
    mavlink_attitude_t attitude;

    // MAVLink config
    // int sysid;
    // int compid;
    // int type;
    // uint8_t autopilot_type;
    // uint8_t system_mode;
    // uint32_t custom_mode;
    // uint8_t system_state;

    // Initialize the required buffers
    mavlink_message_t msg;
    mavlink_status_t status;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
};

extern class Logger logger;

#endif