/**
 *  This program is a UART- SBUS protocol for the Pi Pico 
 *  Derived from SBUS driver from user mmosca
 * 
 * https://github.com/mmosca/pico-sbus/blob/main/sbus/sbus.c
 * 
*/

#include <stdio.h>
#include <string.h>

//#include <assert.h>

#include "sbus.h"

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/sync.h"

volatile uint32_t irq_count = 0;

volatile uint8_t sbus_data[SBUS_FIFO_SIZE][SBUS_MESSAGE_MAX_SIZE];
volatile uint8_t current_sbus_data[SBUS_MESSAGE_MAX_SIZE];
volatile uint8_t oldest = 0;
volatile uint8_t newest = 0;
volatile uint8_t stored = 0;
volatile uint32_t sbus_index = 0;
volatile bool hasStartByte = false;

static uart_inst_t *sbus_uart_id;

critical_section_t fifo_lock;

//#define DEBUG
#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif

#ifdef DEBUG
#define D(X) (X)
#else
#define D(X)
#endif

#define IS_SET(val, bit) (val & (1 << bit))

typedef struct
{
    uint8_t idx;
    uint8_t shift1;
    uint8_t shift2;
    uint8_t shift3; // 11 = byte 3 ignored
} chinfo_t;

static const chinfo_t CHINFO[16] = { {0,  0, 8, 11}, {1,  3, 5, 11}, {2,  6, 2, 10}, {4,  1, 7, 11}, {5,  4, 4, 11},
                                           {6,  7, 1, 9},  {8,  2, 6, 11}, {9,  5, 3, 11}, {11, 0, 8, 11}, {12, 3, 5, 11},
                                           {13, 6, 2, 10}, {15, 1, 7, 11}, {16, 4, 4, 11}, {17, 7, 1, 9},  {19, 2, 6, 11},  {20, 5, 3, 11} };

void decode_sbus_data(const uint8_t *data, sbus_state_t *decoded)
{
    for(uint8_t channel = 0; channel < 16; ++channel)
    {
        const chinfo_t *info = &CHINFO[channel];
        uint8_t idx = info->idx + 1;
        uint8_t b1 = data[idx];
        uint8_t b2 = data[idx+1];
        uint8_t b3 = data[idx+2];

        uint16_t chData = ((b1 >> info->shift1) | (b2 << info->shift2) | (b3 << info->shift3)) & 0x7FF;
        printf("CH %i Data = %i, ", channel, chData);
        //assert(chData < 2048); // Remove to avoid total system stop

        decoded->ch[channel] = chData;
    }

    decoded->ch[16] = data[23] & 1 ? 0x7FF : 0;
    decoded->ch[17] = data[23] & (1<<1) ? 0x7FF : 0;
    decoded->framelost = data[23] & (1<<2);
    decoded->failsafe = data[23] & (1<<3);
}

void sbus_init(uart_inst_t *uart, uint8_t rx_pin, uint8_t tx_pin)
{
    printf("Initializing UART for SBUS\n");
    sbus_uart_id = uart;
    // init mutex
    critical_section_init(&fifo_lock);
    // clear fifo
    for(uint8_t i = 0; i < SBUS_FIFO_SIZE; ++i)
    {
        memset((void *)sbus_data[i], 0, SBUS_MESSAGE_MAX_SIZE);
    }
    oldest = newest = stored = 0;

    uart_init(uart, 115200);

    gpio_set_function(rx_pin, GPIO_FUNC_UART);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);

    uint32_t actual = uart_set_baudrate(uart, SBUS_BAUD_RATE);
    printf("Actual baud rate: %i\n", actual);

    uart_set_hw_flow(uart, false, false);
    uart_set_format(uart, SBUS_DATA_BITS, SBUS_STOP_BITS, SBUS_PARITY);
    uart_set_fifo_enabled(uart, false); // turning off FIFO Doing this char by char

    // Setting up the RX interrupt
    printf("Configuring hardware interrupt for UART\n");
    uint32_t UART_IRQ = uart == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, sbus_on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(uart, true, false);
    printf("Configuration complete!\n\n");
}

bool hasSbusData()
{
    return stored > 0 && oldest != newest;
}

bool readSbusData(uint8_t *data)
{
    bool ret = false;
    critical_section_enter_blocking(&fifo_lock);
    if(hasSbusData())
    {
        memcpy((void *)data, (void *)sbus_data[oldest], SBUS_MESSAGE_MAX_SIZE);
        oldest = (oldest + 1) % SBUS_FIFO_SIZE;
        stored--;
        ret = true;
    }
    critical_section_exit(&fifo_lock);

    return ret;
}

// RX interrupt handler
void sbus_on_uart_rx()
{
    irq_count++;
    while (uart_is_readable(sbus_uart_id))
    {
        uint8_t ch = uart_getc(sbus_uart_id);
        if(!hasStartByte && ch != SBUS_STARTBYTE)
        {
            continue;
        }

        hasStartByte = true;
        current_sbus_data[sbus_index++] = ch;

        if(sbus_index == SBUS_MESSAGE_MAX_SIZE)
        {
            hasStartByte = false;
            sbus_index = 0;

            if(current_sbus_data[SBUS_MESSAGE_MAX_SIZE - 1] == SBUS_ENDBYTE ||
                (current_sbus_data[SBUS_MESSAGE_MAX_SIZE - 1] & SBUS2_ENDBYTE_MASK) == SBUS2_ENDBYTE)
            {
                critical_section_enter_blocking(&fifo_lock);
                uint8_t nextNewest = (newest + 1) % SBUS_FIFO_SIZE;
                memcpy((void *)sbus_data[nextNewest], (void *)current_sbus_data, SBUS_MESSAGE_MAX_SIZE);
                newest = nextNewest;
                if(oldest = nextNewest)
                {
                    oldest = (oldest + 1) % SBUS_FIFO_SIZE;
                }

                stored++;
                if(stored > SBUS_FIFO_SIZE)
                {
                    stored = SBUS_FIFO_SIZE;
                }
                critical_section_exit(&fifo_lock);
            }
        }
    }
}
