/**
 *  This file is part of the pico-autobixler project
 * 
 *  Originaly based off of the betaflight github repo
 *  link: https://github.com/betaflight/betaflight/blob/master/src/main/rx/crsf.h
 * 
 *  Defines variables and functions to use TBS Crossfire Protocol
 * 
 *  Modified by: Michael Gromski
 */

#pragma once

#include "crsf_protocol.h"
#include "crsf.c"

#define STATIC_UNIT_TESTED static

#define CRSF_PORT_OPTIONS   (SERIAL_STOPBITS_1 | SERIAL_PARITY_NO)
#define CRSF_PORT_MODE      MODE_RXTX

#define CRSF_MAX_CHANNEL    16
#define CRSFV3_MAX_CHANNEL  24

#define CRSF_SUBSET_RC_STARTING_CHANNEL_BITS        5
#define CRSF_SUBSET_RC_STARTING_CHANNEL_MASK        0x1F
#define CRSF_SUBSET_RC_RES_CONFIGURATION_BITS       2
#define CRSF_SUBSET_RC_RES_CONFIGURATION_MASK       0x03
#define CRSF_SUBSET_RC_RESERVED_CONFIGURATION_BITS  1

#define CRSF_RC_CHANNEL_SCALE_LEGACY        0.62477120195241f
#define CRSF_SUBSET_RC_RES_CONF_10B         0
#define CRSF_SUBSET_RC_RES_BITS_10B         10
#define CRSF_SUBSET_RC_RES_MASK_10B         0X03FF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_10B    1.0f
#define CRSF_SUBSET_RC_RES_CONF_11B         1
#define CRSF_SUBSET_RC_RES_BITS_11B         11
#define CRSF_SUBSET_RC_RES_MASK_11B         0X07FF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_11B    0.5f
#define CRSF_SUBSET_RC_RES_CONF_12B         2
#define CRSF_SUBSET_RC_RES_BITS_12B         12
#define CRSF_SUBSET_RC_RES_MASK_12B         0X0FFF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_12B    0.25f
#define CRSF_SUBSET_RC_RES_CONF_13B         3
#define CRSF_SUBSET_RC_RES_BITS_13B         13
#define CRSF_SUBSET_RC_RES_MASK_13B         0X1FFF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_13B    0.125f

#define CRSF_RSSI_MIN (-130)
#define CRSF_RSSI_MAX 0
#define CRSF_SNR_MIN (-30)
#define CRSF_SNR_MAX 20

/*
For documentation:
typedef enum
{
    CRSF_RF_MODE_4_FPS = 0,
    CRSF_RF_MODE_50_FPS,
    CRSF_RF_MODE_150_FPS,
} crsfRfMode_e;
*/

typedef struct crsfFrameDef_s 
{
    uint8_t deviceAddress;
    uint8_t frameLength;
    uint8_t type;
    uint8_t payload[CRSF_PAYLOAD_SIZE_MAX + 1]; // +1 for CRC at end of payload
} crsfFrameDef_t;

typedef union crsfFrame_u
{
    uint8_t bytes[CRSF_FRAME_SIZE_MAX];
    crsfFrameDef_t frame;
} crsfFrame_t;

void crsfRxWriteTelemetryData(const void *data, int len);
void crsfRxSendTelemetryData(void);

struct rxConfig_s;
struct rxRuntimeState_s;
bool crsfRxInit(const struct rxConfig_s *initialRxConfig, struct rxRuntimeState_s *rxRuntimeState);
void crsfRxUpdateBaudrate(uint32_t baudrate);
bool crsfRxIsActive(void);