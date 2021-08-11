/**
 * This program will read a non-inverted sbus signal through
 * UART0RX and UART0 TX on GPIO pins 17 and 16 respectively
 * and print the output through serial USB
 * 
 * SBUS drivers from Bolder Flight Systems will be relied on 
 * heavily for this program
 * 
 * Written by: Michael Gromski
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "sbus.h"

#define UART_RX_ID uart1
#define SBUS_BAUD_RATE 100000
#define UART1_TX_PIN 8
#define UART1_RX_PIN 9

uint8_t sbus_data_raw[SBUS_MESSAGE_MAX_SIZE];
sbus_state_t sbus_data;

int main() 
{   
    // Initialize outputs and hardware
    stdio_init_all();
    
    sbus_init(UART_RX_ID, UART1_RX_PIN, UART1_TX_PIN);

    // Dummy print output to USB
    printf("Reading SBUS RX Signal...\n");
    while (true) 
    {
        if (hasSbusData() == true)
        {
            readSbusData(sbus_data_raw);
            decode_sbus_data(sbus_data_raw, &sbus_data);
        }
        for (uint8_t i = 0; i<=24; ++i)
        {
            printf(",%i ", sbus_data_raw[i]);
        }
        printf("\n");
        sleep_ms(1000);
    }
    return 0;
}
