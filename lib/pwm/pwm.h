#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/sync.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#include "system.h"

void servo_init(uint8_t ch_num[]); 
void servo_write(uint8_t ch_num, uint16_t pwm_cycle);
uint16_t angle_to_pwm_cycle(float angle_input); 
float sbus_rx_to_angle(uint16_t *rx_input);
uint16_t sbus_rx_to_pwm_cycle(uint16_t rx_input);
