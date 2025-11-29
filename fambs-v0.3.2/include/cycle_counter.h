#ifndef CYCLE_COUNTER_H
#define CYCLE_COUNTER_H

#include <stdint.h>

static inline uint64_t rdcycle(void) {
    uint64_t x;
    __asm__ volatile ("rdcycle %0" : "=r"(x));
    return x;
}

#endif
