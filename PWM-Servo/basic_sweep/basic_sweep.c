/**
 * This program creates PWM signals to sweep back and forth
 * 
 * Written by: Michael Gromski
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"

int servoInput[] = {0,0,0,0,0}; // {aleron, elevator, throttle, rudder, extra}

void writeServo(int* inValArray)
{
    // Do something
}

int main ()
{
    // configure pins for gpio pwm
    gpio_set_function(18, GPIO_FUNC_PWM); //18 - 22
    /*
    gpio_set_function(19, GPIO_FUNC_PWM);
    gpio_set_function(20, GPIO_FUNC_PWM);
    gpio_set_function(21, GPIO_FUNC_PWM);
    gpio_set_function(22, GPIO_FUNC_PWM);
    */
    
    // add array for servo inputs (0-255 for now)
    for(int i=0; i<255; i++)
    {
        servoInput[0] = i;
        writeServo(servoInput);
    }
}

