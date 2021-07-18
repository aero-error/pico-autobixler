/**
 * This program creates PWM signals to sweep back and forth
 * 
 * PWM signal sends a pulse every 20ms but the command signal
 * is only 1-2 ms long. 1000us for -90 deg, 1500us for 0 deg,
 * 2000us for +90 deg 
 * 
 * Currently set up for 1 CH but will be modified for 6 channels
 * 
 * PWM Channel Arrangment:
 * GPIO18 -> CH1 -> Aileron
 * GPIO19 -> CH2 -> Elevator
 * GPIO20 -> CH3 -> Throttle
 * GPIO21 -> CH4 -> Rudder
 * GPIO22 -> CH5 -> AUX
 * GPIO28 -> CH6 -> AUX
 * 
 * Written by: Michael Gromski
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define ch_aileron_pin 18
#define ch_elevator_pin 19
#define ch_throttle_pin 20
#define ch_rudder_pin 21
#define ch_aux1_pin 22
#define ch_aux2_pin 23

uint16_t servoInput[] = {0,0,0,0,0,0}; // {aleron, elevator, throttle, rudder, aux, aux}

uint16_t wrap = 32768;


void writeServo(uint16_t *inValArray)
{
    // Do something
    uint16_t PWM_pulse_length = 20000; 
}

int main ()
{
    // Lets do some system configuration
    // Set system clock to 125MHZ
    set_sys_clock_khz(125000,true);

    // configure pins for gpio pwm
    gpio_set_function(ch_aileron_pin, GPIO_FUNC_PWM); //18 - 22
    /*
    gpio_set_function(ch_elevator_pin, GPIO_FUNC_PWM);
    gpio_set_function(ch_throttle_pin, GPIO_FUNC_PWM);
    gpio_set_function(ch_rudder_pin, GPIO_FUNC_PWM);
    gpio_set_function(ch_aux1_pin, GPIO_FUNC_PWM);
    gpio_set_function(ch_aux2_pin, GPIO_FUNC_PWM)
    */

    // Figure out which pwm slice we are connected to 
    uint slice_num_ch1 = pwm_gpio_to_slice_num(ch_aileron_pin);

    pwm_set_wrap(slice_num_ch1, wrap);
    pwm_set_gpio_level(ch_aileron_pin, 2500);
    pwm_set_enabled(slice_num_ch1, true);
}

