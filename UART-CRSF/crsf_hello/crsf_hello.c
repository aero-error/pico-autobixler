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
#include "crsf.h"

#define UART_RX_ID uart0
#define UART0_TX_PIN 16
#define UART0_RX_PIN 17

uint32_t rx_output[] = {0,0,0,0,0,0};

int main() 
{   
    // Initialize outputs and hardware
    stdio_init_all();
    /*
    uart_init(UART_RX_ID, BAUD_RATE);

    // Configure UART pins
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);

    // CREATE READ UART SBUS CODE HERE

    // Dummy print output to USB
    printf("Reading SBUS RX Signal...\n");
    while (true) 
    {
        for (int i = 0; i <=5; i++)
        {
            printf("%i,\t",rx_output[i]);
            rx_output[i] += 1;
        }
        printf("\n");
        sleep_ms(1000);
    }
    */
    return 0;
}
