#pragma once 

#include "pico/stdlib.h"
#include "pico/sync.h"

#define SYS_CLOCK_FREQ_KHZ 125000

critical_section_t fifo_lock;
