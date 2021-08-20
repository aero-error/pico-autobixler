/**
 * This program will read from the BMP388 barometer on
 * each interupt, process the raw data, and print the 
 * results
 * 
 * NOTE: SPI speeds should be clock speed divided by
 * integers ex: 125 MHZ / 50 = 2.5MHz
 * 
 * Written by: Michael Gromski
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define SPI_BMP388 spi1
#define SPI1_SPEED 500000

#define PIN_SCK 10
#define PIN_MOSI 11
#define PIN_MISO 12
#define PIN_CS 13

uint8_t write_buff[8];
unsigned char read_buff[8];

int main()
{
    stdio_init_all();
    printf("Start of Stream!\n\n");

    // Configure SPI1 for 2.5 MHz
    printf("Configuring SPI1 Interface...\n");
    spi_init(SPI_BMP388, SPI1_SPEED);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, true);
    printf("Configuration Done!\n");

    volatile uint32_t baudrate = spi_get_baudrate(SPI_BMP388);
    printf("SPI1 Baudrate: %i\n", baudrate);

    write_buff[0] = 0x00;

    // pull CS pin low to read
    gpio_put(PIN_CS, false);
    
    while (true)
    {
        if (spi_is_readable(SPI_BMP388) == true)
        {
            printf("SPI1 is readable!\n");
        }
        else
        {
            printf("SPI1 is not readable\n");
        }

        if (spi_is_writable(SPI_BMP388) == true)
        {
            printf("SPI1 is writeable!\n");
        }
        else
        {
            printf("SPI1 is not writeable\n");
        }

        printf("Reading BMP388 Chip ID\n");

        spi_write_read_blocking(SPI_BMP388, write_buff, read_buff, 1);

        printf("Result: %i\n\n",read_buff[0]);
        sleep_ms(500);
    }
    return 0;
}