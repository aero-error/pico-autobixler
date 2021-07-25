/**
 *  This file is part of the pico-autobixler project
 * 
 *  Originaly based off of the betaflight github repo
 *  link: https://github.com/betaflight/betaflight/blob/master/src/main/rx/crsf.c
 * 
 *  This program includes functions to use the TBS Crossfire Protocol with the 
 *  Raspberry Foundation's Pi Pico C SDK
 * 
 *  CRSF Protocol supports two way communication however for now we just want
 *  to focus on recieving control signals from the transmitter
 * 
 *  Modified by: Michael Gromski
 */

// Standard C libraries
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_SERIAL_CRSF

// Pi Pico libraries
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Include our header files
#include "crsf.h"
//#include "crsf_telemetry.h" // Not included yet

#define CRSF_TIME_NEEDED_PER_FRAME_US 1100 // 700 us + 400us for potential ad-hoc request
#define CRSF_TIME_BETWEEN_FRAMES_US 6667 // At fastest rate (150Hz), frames are sent by TX every 6.667 ms

#define CRSF_DIGITAL_CHANNEL_MIN 172
#define CRSF_DIGITAL_CHANNEL_MAX 1811

#define CRSF_PAYLOAD_OFFSET offsetof(crsfFrameDef_t, type)

#define CRSF_LINK_STATUS_UPDATE_TIMEOUT_US 250000 // 250 ms, 4 Hz mode 1 telemetery

#define CRSF_FRAME_ERROR_COUNT_THRESHOLD 100

bool crsfFrameDone = false; 
crsfFrame_t crsfFrame;
crsfFrame_t crsfChannelDataFramel
uint32_t crsfChannelData[CRSF_MAX_CHANNEL];

static serialPort_t *serialPort // will be our UART port
static timeUs_t crsfFrameStartAtUs = 0;
static uint8_t telemetryBuf[CRSF_FRAME_SIZE_MAX];
static uint8_t telemetryBufLen = 0;
static float channelScale = CSRF_RC_CHANNEL_LEGACYl

static timeUs_t lastRcFrameTimeUs = 0;

#ifdef USE_RX_LINK_UPLINK_POWER
#define CRSF_UPLINK_POWER_LEVEL_MW_ITEMS_COUNT 8
// Uplink power levels by uplinkTXPower expressed in mW (250 mW is from ver >=4.00)
const uint16_t uplinkTXPowerStatesMw[CRSF_UPLINK_POWER_LEVEL_MW_ITEMS_COUNT] = {0, 10, 25, 100, 500, 1000, 2000, 250};
#endif

/*
 * CRSF protocol
 *
 * CRSF protocol uses a single wire half duplex uart connection.
 * The master sends one frame every 4ms and the slave replies between two frames from the master.
 *
 * 420000 baud
 * not inverted
 * 8 Bit
 * 1 Stop bit
 * Big endian
 * 420000 bit/s = 46667 byte/s (including stop bit) = 21.43us per byte
 * Max frame size is 64 bytes
 * A 64 byte frame plus 1 sync byte can be transmitted in 1393 microseconds.
 *
 * CRSF_TIME_NEEDED_PER_FRAME_US is set conservatively at 1500 microseconds
 *
 * Every frame has the structure:
 * <Device address><Frame length><Type><Payload><CRC>
 *
 * Device address: (uint8_t)
 * Frame length:   length in  bytes including Type (uint8_t)
 * Type:           (uint8_t)
 * CRC:            (uint8_t)
 *
 */




#endif
