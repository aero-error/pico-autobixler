/**
 * This file is part of the pico-autobixler project
 * 
 * Originaly based off of the betaflight github repo
 * link: https://github.com/betaflight/betaflight/blob/master/src/main/rx/crsf_protocol.h
 * 
 * Defines variables/commands used by the TBS CrossFire protocol
 * 
 * Modified by: Michael Gromski
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CRSF_BAUDRATE 420000 // nice 

enum { CRSF_SYNC_BYTE  = 0xC8 };

enum { CRSF_FRAME_SIZE_MAX = 64 }; // 62 byte frame with 2 header bytes (<length><type>)
enum { CRSF_PAYLOAD_SIZE_MAX = CRSF_FRAME_SIZE_MAX - 6 };

typedef enum
{
    CRSF_FRAMETYPE_GPS = 0x02,
    CRSF_FRAMETYPE_BATTERY_SENSOR = 0x08,
    CRSF_FRAMETYPE_LINK_STATISTICS = 0x14,
    CRSF_FRAMETYPE_RX_CHANNELS_PACKED = 0x16,
    CRSF_FRAMETYPE_SUBSET_RC_CHANNELS_PACKED = 0x17,
    CRSF_FRAMETYPE_LINK_STATISTICS_RX = 0x1C,
    CRSF_FRAMETYPE_LINK_STATISTICS_TX = 0x1D,
    CRSF_FRAMETYPE_ATTITUDE = 0x1E,
    CRSF_FRAMETYPE_FLIGHT_MODE = 0x21,
    // Extended header frames, range 0x28 - 0x96
    CRSF_FRAMETYPE_DEVICE_PING = 0x28,
    CRSF_FRAMETYPE_DEVICE_INFO = 0x29,
    CRSF_FRAMETYPE_PARAMETER_SETTINGS_ENTRY = 0x2B,
    CRSF_FRAMETYPE_PARAMETER_READ = 0x2C,
    CRSF_FRAMETYPE_PARAMETER_WRITE = 0x2D,
    CRSF_FRAMETYPE_COMMAND = 0x32,
    // MSP commands
    CRSF_FRAMETYPE_MSP_REQ = 0x7A,  // response request
    CRSF_FRAMETYPE_MSP_RESP = 0x7B, // reply with 58 byte chunked binary
    CRSF_FRAMETYPE_MSP_WRITE = 0x7C, //write with 8 byte chunked binary (OpenTx outbound telemetry buffer list)
    CRSF_FRAMETYPE_DISPLAYPORT_CMD = 0x7D,
} crsfFrameType_e;

enum 
{
    CRSF_COMMAND_SUBCMD_GENERAL = 0x0A, // general command
};

enum
{
    CRSF_COMMAND_SUBCMD_GENERAL_CRSF_SPEED_PROPOSAL = 0x70, // proposed new CRSF port speed
    CRSF_COMMAND_SUBCMD_GENERAL_CRSF_SPEED_RESPONSE = 0x71, // response to the proposed CRSF port speed
};

enum
{
    CRSF_DISPLAYPORT_SUBCMD_UPDATE = 0x01, // transmit displayport buffer to remote
    CRSF_DISPLAYPORT_SUBCMD_CLEAR = 0x02, // clear client screen
    CRSF_DISPLAYPORT_SUBCMD_OPEN = 0x03, // client request to open cms menu
    CRSF_DISPLAYPORT_SUBCMD_CLOSE = 0x04, // client request to close cms menu
    CRSF_DISPLAYPORT_SUBCMD_POLL = 0x05, // client request to poll/refresh cms menu
};

enum 
{
    CRSF_DISPLAYPORT_OPEN_ROWS_OFFSET = 1,
    CRSF_DISPLAYPORT_OPEN_COLS_OFFSET = 2,
};

enum
{
    CRSF_FRAME_GPS_PAYLOAD_SIZE = 15,
    CRSF_FRAME_BATTERY_SENSOR_PAYLOAD_SIZE = 8,
    CRSF_FRAME_LINK_STATISTICS_PAYLOAD_SIZE = 10,
    CRSF_FRAME_LINK_STATISTICS_TX_PAYLOAD_SIZE = 6,
    CRSF_FRAME_RC_CHANNELS_PAYLOAD_SIZE = 22, // 11 bits per channel * 16 channels = 22 bytes
    CRSF_FRAME_ATTITUDE_PAYLOAD_SIZE = 6,
};

enum
{
    CRSF_FRAME_LENGTH_ADDRESS = 1, // length of address field
    CRSF_FRAME_LENGTH_FRAMELENGTH = 1, // length of FRAMELENGTH field
    CRSF_FRAME_LENGTH_TYPE = 1, // length of TYPW field
    CRSF_FRAME_LENGTH_CRC = 1, // length of CRC field
    CRSF_FRAME_LENGTH_TYPE_CRC = 2, // length of TYPE and CRC field combined
    CRSF_FRAME_LENGTH_EXT_TYPE_CRC = 4, // length of Extended Dest/Origin, TYPE and CRC fields combined
    CRSF_FRAME_LENGTH_NON_PAYLOAD = 4, // combined length of all field except for payload
};

enum
{
    CRSF_FRAME_TX_MSP_FRAME_SIZE = 58,
    CRSF_FRAME_RX_MSP_FRAME_SIZE = 8, 
    CRSF_FRAME_ORIGIN_DEST_SIZE = 2,
};

// Clashes with CRSF_ADDRESS_FLIGHT_CONTROLLER
#define CRSF_SYNC_BYTE 0xC8

typedef enum
{
    CRSF_ADDRESS_BROADCAST = 0x00,
    CRSF_ADDRESS_USB = 0x10,
    CRSF_ADDRESS_TBS_CORE_PNP_PRO = 0x80,
    CRSF_ADDRESS_RESERVED1 = 0x8A,
    CRSF_ADDRESS_CURRENT_SENSOR = 0xC0,
    CRSF_ADDRESS_GPS = 0xC2,
    CRSF_ADDRESS_TBS_BLACKBOX = 0xC4,
    CRSF_ADDRESS_FLIGHT_CONTROLLER = 0xC8,
    CRSF_ADDRESS_RESERVED2 = 0xCA,
    CRSF_ADDRESS_RACE_TAG = 0xCC,
    CRSF_ADDRESS_RADIO_TRANSMITTER = 0xEA,
    CRSF_ADDRESS_CRSF_RECIEVER = 0xEC,
    CRSF_ADDRESS_CRSF_TRANSMITTER = 0xEE,
} crsfAddress_e;