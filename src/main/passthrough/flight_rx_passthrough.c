/**
 * BRIEF:This is a program intended for use in flight
 * with a small UAV (~1kg, 1m wingspan, <300W motor). 
 * 
 * DESCRIPTION:Will read non-inverted SBUS signal 
 * (needs NOT gate) from a FrSky or TBS Crossfire RX 
 * (in SBUS mode) and pass the RX input straight to 
 * the PWM output to the servos.
 * 
 * Will make the Pi Pico to act as sort of a passthrough
 * SBUS to PWM conversion device.
 * 
 * This is the foundation that the autopilot will be built
 * upon and act as a pilot correctable failsafe mode incase
 * of emergency or failure.
 * 
 * NOTE: This is a single thread program. Future releases
 * may include multithreading/multicore for Pi Pico.
 * 
 * WARNING: Code is not fully tested yet!
 * 
 * AUTHOR: Michael Gromski
 */

// Include needed libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "sbus.h"
#include "pwm.h"

/*** DEFINITIONS START ***/
// SBUS UART definitions

// Flight Hardware
#define SBUS_UART_ID uart0
#define SBUS_TX_PIN 16
#define SBUS_RX_PIN 17

/*
// Debug
#define SBUS_UART_ID uart1
#define SBUS_TX_PIN 8
#define SBUS_RX_PIN 9
*/

// PWM servo definitions 
#define CH_AILERON_PIN 18
#define CH_ELEVATOR_PIN 19
#define CH_THROTTLE_PIN 20
#define CH_RUDDER_PIN 21
#define CH_AUX1_PIN 22
#define CH_AUX2_PIN 28

uint8_t ch_num[] = {CH_AILERON_PIN, CH_ELEVATOR_PIN, CH_THROTTLE_PIN, CH_RUDDER_PIN, CH_AUX1_PIN, CH_AUX2_PIN};
uint8_t sbus_data[SBUS_MESSAGE_MAX_SIZE];
sbus_state_t sbus_data_processed;

/*** DEFINITIONS END ***/
/*** MAIN START ***/
int main()
{
    /*** SETUP START ***/
    stdio_init_all();
    servo_init(ch_num);
    sbus_init(SBUS_UART_ID, SBUS_RX_PIN, SBUS_TX_PIN);   
    /*** SETUP END ***/

    /*** LOOP START ***/
    while (true)
    {
        decode_sbus_data(sbus_data, &sbus_data_processed);
        // Normal operation
        if(sbus_data_processed.failsafe == false)
        {
            for(uint8_t i = 0; i < 6; ++i)
            {
                servo_write(ch_num[i], sbus_rx_to_pwm_cycle(sbus_data_processed.ch[i]));
            }
        }
        // Failsafe proceedure is to center control surfaces and throttle to 0%
        else 
        {
            servo_write(CH_AILERON_PIN, angle_to_pwm_cycle(0.0f));
            servo_write(CH_ELEVATOR_PIN,angle_to_pwm_cycle(0.0f));
            servo_write(CH_THROTTLE_PIN, angle_to_pwm_cycle(-180.0f));
            servo_write(CH_RUDDER_PIN, angle_to_pwm_cycle(0.0f));
            servo_write(CH_AUX1_PIN, angle_to_pwm_cycle(0.0f));
            servo_write(CH_AUX2_PIN, angle_to_pwm_cycle(0.0f));
        }
        
    }
    /*** LOOP END ***/
    return 0;
}
/*** MAIN END ***/