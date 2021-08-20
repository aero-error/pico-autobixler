/**
 * This is a PWM servo driver for the Pi Pico 
 * Using functions defined in the C SDK
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
 * Pulse[ms] -- Angle [deg] -- Cycles [count]
 * 0.5ms -- -180 -- 819
 * 1.0ms -- -90 -- 1638
 * 1.5ms -- 0 -- 2458
 * 2.0ms -- +90 -- 3277
 * 2.5ms -- +180 -- 4096
 * 
 * Written by: Michael Gromski
 */

#include "pwm.h"

volatile uint16_t pwm_clock_wrap = 32768;
volatile float pwm_clock_div = 76.3f;

// Configures system clock, PWM clock & counter, and servo pins
void servo_init(uint8_t ch_num[]) 
{
    // Set system clock to 125MHZ (DO NOT CHANGE THIS VALUE!)
    set_sys_clock_khz(SYS_CLOCK_FREQ_KHZ,true);
    
    uint8_t slice_ch_num[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    // Configure the PWM clock for servo output
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, pwm_clock_div);
    pwm_config_set_wrap(&config, pwm_clock_wrap);

    // configure pins for GPIO PWM, get their slice number, and initialize
    for (int i = 0; i < 6; i++)
    {
        gpio_set_function(ch_num[i], GPIO_FUNC_PWM);
        slice_ch_num[i] = pwm_gpio_to_slice_num(ch_num[i]);
        pwm_init(slice_ch_num[i], &config, true);
    }
}

void servo_write(uint8_t ch_num, uint16_t pwm_cycle)
{
    critical_section_enter_blocking(&fifo_lock);
    pwm_set_gpio_level(ch_num, pwm_cycle);
    critical_section_exit(&fifo_lock);
}

// Converts servo deg to cycle format
uint16_t angle_to_pwm_cycle(float angle_input) 
{
    float cycle_output = 9.1033f * angle_input + 2457.6f;
    return (uint16_t)cycle_output;
}

// Converts raw sbus rx signal to degrees
float sbus_rx_to_angle(uint16_t *rx_input)
{
    float angle_output = 0.17578125f * *rx_input - 180.0f;
    return angle_output;
}

uint16_t sbus_rx_to_pwm_cycle(uint16_t rx_input)
{
    float cycle_output = 1.600821f * rx_input + 818.68f;
    return (uint16_t)cycle_output;
}