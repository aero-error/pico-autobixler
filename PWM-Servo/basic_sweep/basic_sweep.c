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

float servoInput[] = {0,0,0,0,0,0}; // {aleron, elevator, throttle, rudder, aux, aux}

uint16_t wrap = 32768;


void writeServo(uint16_t *inValArray)
{
    // Do something
    uint16_t PWM_pulse_length = 20000; 
}

uint16_t angle_to_cycle(float *angleInput)
{
    float cycleOutput = 9.1033 * *angleInput + 2457.6;
    return (uint16_t)cycleOutput;
}

int main ()
{
    // Lets do some system configuration
    // Set system clock to 125MHZ (DO NOT CHANGE THIS VALUE!)
    set_sys_clock_khz(125000,true);

    // configure pins for gpio pwm
    gpio_set_function(ch_aileron_pin, GPIO_FUNC_PWM); //18 - 22
    gpio_set_function(ch_elevator_pin, GPIO_FUNC_PWM);
    gpio_set_function(ch_throttle_pin, GPIO_FUNC_PWM);
    gpio_set_function(ch_rudder_pin, GPIO_FUNC_PWM);
    gpio_set_function(ch_aux1_pin, GPIO_FUNC_PWM);
    gpio_set_function(ch_aux2_pin, GPIO_FUNC_PWM);

    // Figure out which pwm slice we are connected to 
    uint8_t slice_num_ch1 = pwm_gpio_to_slice_num(ch_aileron_pin);
    uint8_t slice_num_ch2 = pwm_gpio_to_slice_num(ch_elevator_pin);
    uint8_t slice_num_ch3 = pwm_gpio_to_slice_num(ch_throttle_pin);
    uint8_t slice_num_ch4 = pwm_gpio_to_slice_num(ch_rudder_pin);
    uint8_t slice_num_ch5 = pwm_gpio_to_slice_num(ch_aux1_pin);
    uint8_t slice_num_ch6 = pwm_gpio_to_slice_num(ch_aux2_pin);

    //
    pwm_set_clkdiv(slice_num_ch1, 76.3f);
    pwm_set_wrap(slice_num_ch1, wrap);
    pwm_set_gpio_level(ch_aileron_pin, 2500);
    pwm_set_enabled(slice_num_ch1, true);
    
    /** Diffrent PWM levels
     * 0.5ms -- -180 -- 819
     * 1.0ms -- -90 -- 1638
     * 1.5ms -- 0 -- 2458
     * 2.0ms -- +90 -- 3277
     * 2.5ms -- +180 -- 4096
     */
    servoInput[0] = -90;
    while (true)
    {
        if (servoInput[0] < 89)
        {
            pwm_set_gpio_level(ch_aileron_pin, angle_to_cycle(&servoInput[0]));
            servoInput[0] += 0.1;
        }
        else
        {
            servoInput[0] = -90;
        }
        sleep_ms(20);
    }
}

