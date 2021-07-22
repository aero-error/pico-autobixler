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
 * PIN -> CH -> Index -> Control Surface
 * GPIO18 -> CH1 -> 0 -> Aileron
 * GPIO19 -> CH2 -> 1 -> Elevator
 * GPIO20 -> CH3 -> 2 -> Throttle
 * GPIO21 -> CH4 -> 3 -> Rudder
 * GPIO22 -> CH5 -> 4 -> AUX
 * GPIO28 -> CH6 -> 5 -> AUX
 * 
 * Different PWM levels
 * 0.5ms -- -180 -- 819
 * 1.0ms -- -90 -- 1638
 * 1.5ms -- 0 -- 2458
 * 2.0ms -- +90 -- 3277
 * 2.5ms -- +180 -- 4096
 * 
 * Written by: Michael Gromski
 */

// Include needed libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Define global variables
#define ch_aileron_pin 18
#define ch_elevator_pin 19
#define ch_throttle_pin 20
#define ch_rudder_pin 21
#define ch_aux1_pin 22
#define ch_aux2_pin 23

float servoInput[] = {0,0,0,0,0,0}; // {aleron, elevator, throttle, rudder, aux, aux}
uint8_t ch_num[] = {ch_aileron_pin, ch_elevator_pin, ch_throttle_pin, ch_rudder_pin, ch_aux1_pin, ch_aux2_pin};
uint8_t slice_ch_num[] = {0,0,0,0,0,0};
uint16_t wrap = 32768;

// Define Functions
uint16_t angle_to_cycle(float *angleInput) // Converts servo deg to cycle format
{
    float cycleOutput = 9.1033 * *angleInput + 2457.6;
    return (uint16_t)cycleOutput;
}

// Configures system clock, PWM clock and counter, and servo pins
void servo_init() 
{
    // Lets do some system configuration
    // Set system clock to 125MHZ (DO NOT CHANGE THIS VALUE!)
    set_sys_clock_khz(125000,true);

    // Configure the PWM clock for servo output
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 76.3f);
    pwm_config_set_wrap(&config, wrap);

    // configure pins for GPIO PWM, get their slice number, and initialize
    for (int i = 0; i < 6; i++)
    {
        gpio_set_function(ch_num[i], GPIO_FUNC_PWM);
        slice_ch_num[i] = pwm_gpio_to_slice_num(ch_num[i]);
        pwm_init(slice_ch_num[i], &config, true);
    }
}

int main ()
{
    servo_init(); // Set up system clock
    
    for(int i = 0; i <5; i++)
    {
        servoInput[i] = 0;
    }
    
    bool going_up[] = {true, true, true, true, true, true};

    while (true)
    {
        for(uint8_t servo = 0; servo <= 5; servo++)
        {
            if (servoInput[servo] <= 90 & going_up[servo] == true)
            {
                pwm_set_gpio_level(ch_num[servo], angle_to_cycle(&servoInput[servo]));
                servoInput[servo] += 0.01;
                if (servoInput[servo] >= 90)
                {
                    going_up[servo] = false;
                }
            }
            if (servoInput[servo] >= -90 & going_up[servo] == false)
            {
                pwm_set_gpio_level(ch_num[servo], angle_to_cycle(&servoInput[servo]));
                servoInput[servo] -= 0.01;
                if (servoInput[servo] <= -90)
                {
                    going_up[servo] = true;
                }
            }
            
        }
        sleep_ms(0.5);
    }
}

