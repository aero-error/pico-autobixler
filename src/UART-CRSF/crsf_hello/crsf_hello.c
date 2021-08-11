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
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "crsf.h"

#define UART_RX_ID uart0
#define UART0_TX_PIN 16
#define UART0_RX_PIN 17
#define DATA_BITS 512
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

uint32_t rx_output[] = {0,0,0,0,0,0};
uint8_t buf[64];

void on_uart_rx()
{
    while (uart_is_readable(UART_RX_ID))
    {
        uart_read_blocking(UART_RX_ID, buf, 64);
    }
}

int main() 
{   
    // Initialize outputs and hardware
    stdio_init_all();
    
    uart_init(UART_RX_ID, CRSF_BAUDRATE);

    // Configure UART pins
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);
    
    /*
    // Setting the UART flow control of CTS/RTS, we are disabling these
    uart_set_hw_flow(UART_RX_ID, false, false);

    // Configure our packet format
    uart_set_format(UART_RX_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off fifo - doing this char by char
    uart_set_fifo_enabled(UART_RX_ID, false);

    // Configuring the UART IRQ
    // Settign up the handler first
    int UART_IRQ = UART_RX_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // Setting up and enabling the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Enabling the UART to send interrupts - RX only
    uart_set_irq_enables(UART_RX_ID, true, false);

    // Setup should be complete by now
    */
    // Dummy print output to USB
    printf("Reading SBUS RX Signal...\n");
    while (true) 
    {
        if (uart_is_readable_within_us(UART_RX_ID, 1500))
        {
            printf("UART %s is readable!\n", UART_RX_ID);
        }
        else
        {
            printf("ERROR:UART %s is not readable!\n", UART_RX_ID);
        }
        
        printf("\nPrinting RX bufffer...\n");
        for (int i = 0; i <= 64; i++)
        {
            printf("%i,\t",buf[i]);
        }
        printf("\n");
    }
    
    return 0;
}
