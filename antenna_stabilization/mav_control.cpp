/*
 * MAVLink class implementation
 *
 * Реализация методов класса MAVLink для обмена сообщениями.
 *
 * Автор: BleynChannel (Golovin Vladislav)
 */
#include "mav_control.h"
#include "logger.h"

#define NUM_HBS 60 // # of heartbeats to wait before activating STREAMS from Pixhawk. 60 = one minute.

MAVControl::MAVControl(uint8_t port) : serial(port) {
    // Mavlink variables
    prevMillis = 0; // will store last time MAVLink was transmitted and listened
    numHbsPasados = NUM_HBS;
}

void MAVControl::init(uint8_t rxPin, uint8_t txPin, uint32_t baud) {
    serial.begin(baud, SERIAL_8N1, rxPin, txPin);
}

void MAVControl::tick(uint32_t interval) {
    // MAVLink config
    /* The default UART header for your MCU */ 
    // sysid = 1;                   ///< ID 20 for this airplane. 1 PX, 255 ground station
    // compid = 158;                ///< The component sending the message
    // type = MAV_TYPE_QUADROTOR;   ///< This system is an airplane / fixed wing
    
    // Define the system type, in this case an airplane -> on-board controller
    // system_type = MAV_TYPE_GENERIC;
    // autopilot_type = MAV_AUTOPILOT_INVALID;
    
    // system_mode = MAV_MODE_PREFLIGHT; ///< Booting up
    // custom_mode = 0;                 ///< Custom mode, can be defined by user/adopter
    // system_state = MAV_STATE_STANDBY; ///< System ready for flight

    // Pack the message
    mavlink_msg_heartbeat_pack(1,0, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_INVALID, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
    
    // Copy the message to the send buffer
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    
    // Send the message with the standard UART send function
    // uart0_send might be named differently depending on
    // the individual microcontroller / library in use.
    uint64_t curMillis = millis();
    if (curMillis - prevMillis >= interval) {
        // Timing variables
        prevMillis = curMillis;

        serial.write(buf, len);

        numHbsPasados++;
        if(numHbsPasados>=NUM_HBS) {
            // Request streams from Pixhawk
            requestData();
            numHbsPasados=0;
        }
    }

    // Check reception buffer
    receiveData();
}

void MAVControl::requestData()
{
  // STREAMS that can be requested
  /*
   * Definitions are in common.h: enum MAV_DATA_STREAM
   *   
   * MAV_DATA_STREAM_ALL=0, // Enable all data streams
   * MAV_DATA_STREAM_RAW_SENSORS=1, /* Enable IMU_RAW, GPS_RAW, GPS_STATUS packets.
   * MAV_DATA_STREAM_EXTENDED_STATUS=2, /* Enable GPS_STATUS, CONTROL_STATUS, AUX_STATUS
   * MAV_DATA_STREAM_RC_CHANNELS=3, /* Enable RC_CHANNELS_SCALED, RC_CHANNELS_RAW, SERVO_OUTPUT_RAW
   * MAV_DATA_STREAM_RAW_CONTROLLER=4, /* Enable ATTITUDE_CONTROLLER_OUTPUT, POSITION_CONTROLLER_OUTPUT, NAV_CONTROLLER_OUTPUT.
   * MAV_DATA_STREAM_POSITION=6, /* Enable LOCAL_POSITION, GLOBAL_POSITION/GLOBAL_POSITION_INT messages.
   * MAV_DATA_STREAM_EXTRA1=10, /* Dependent on the autopilot
   * MAV_DATA_STREAM_EXTRA2=11, /* Dependent on the autopilot
   * MAV_DATA_STREAM_EXTRA3=12, /* Dependent on the autopilot
   * MAV_DATA_STREAM_ENUM_END=13,
   * 
   * Data in PixHawk available in:
   *  - Battery, amperage and voltage (SYS_STATUS) in MAV_DATA_STREAM_EXTENDED_STATUS
   *  - Gyro info (IMU_SCALED) in MAV_DATA_STREAM_EXTRA1
   */

  // To be setup according to the needed information to be requested from the Pixhawk
  const int  maxStreams = 2;
  const uint8_t MAVStreams[maxStreams] = {MAV_DATA_STREAM_EXTRA1, MAV_DATA_STREAM_EXTRA2}; // {MAV_DATA_STREAM_EXTENDED_STATUS, MAV_DATA_STREAM_EXTRA1};
  const uint16_t MAVRates[maxStreams] = {0x05, 0x05}; // {0x02,0x05};
    
  for (int i=0; i < maxStreams; i++) {
    /*
     * mavlink_msg_request_data_stream_pack(system_id, component_id, 
     *    &msg, 
     *    target_system, target_component, 
     *    MAV_DATA_STREAM_POSITION, 10000000, 1);
     *    
     * mavlink_msg_request_data_stream_pack(uint8_t system_id, uint8_t component_id, 
     *    mavlink_message_t* msg,
     *    uint8_t target_system, uint8_t target_component, uint8_t req_stream_id, 
     *    uint16_t req_message_rate, uint8_t start_stop)
     * 
     */
    mavlink_msg_request_data_stream_pack(2, 200, &msg, 1, 0, MAVStreams[i], MAVRates[i], 1);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    serial.write(buf, len);
  }
}

void MAVControl::receiveData() {
    // logger.println(mavSerial.available() ? "TRUE" : "FALSE");
    
    while (serial.available()) {
        uint8_t c = serial.read();
        if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
            switch (msg.msgid) {
            case MAVLINK_MSG_ID_VFR_HUD:
                mavlink_msg_vfr_hud_decode(&msg, &vfrHud);
                break;
            
            case MAVLINK_MSG_ID_ATTITUDE:
                mavlink_msg_attitude_decode(&msg, &attitude);
                break;
            
            default:
                break;
            }
        }
    }
}

mavlink_vfr_hud_t MAVControl::getVFRHud() {
    return vfrHud;
}

mavlink_attitude_t MAVControl::getAttitude() {
    return attitude;
}
