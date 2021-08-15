/**
 * This program will read a non-inverted sbus signal through
 * UART0RX and UART0 TX on GPIO pins 17 and 16 respectively
 * and print the output through serial USB
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

uint8_t sbus_data[SBUS_MESSAGE_MAX_SIZE];
sbus_state_t sbus_data_processed;


int main() 
{   
    // Initialize outputs and hardware
    stdio_init_all();
    printf("\n--------------------------\n");
    sbus_init(UART_RX_ID, UART1_RX_PIN, UART1_TX_PIN);
    while(true)
    {
        printf("Raw SBUS Data:\n");
        for (uint8_t i = 0; i < SBUS_MESSAGE_MAX_SIZE; ++i)
        {
            printf("%i, ",sbus_data[i]);
        }

        printf("\nDecoded SBUS Data:\n");
        decode_sbus_data(sbus_data, &sbus_data_processed);
        for (uint8_t i = 0; i < 16; ++i)
        {
            printf("%i, ",sbus_data_processed.ch[i]);
        }
        printf("\n");
        printf("Failsafe Status: %i\n", sbus_data_processed.failsafe);
        printf("Framelost Status: %i\n", sbus_data_processed.framelost);
        printf("\n");
    }
    return 0;
}
