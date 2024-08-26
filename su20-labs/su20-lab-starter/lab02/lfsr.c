#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"
#define ZEROMASK 0x0001
#define TWOMASK 0x0004
#define THREEMASK 0x0008
#define FIVEMASK 0x0020

void lfsr_calculate(uint16_t *reg)
{
    uint16_t zero = (*reg) & ZEROMASK;
    uint16_t two = ((*reg) & TWOMASK) >> 2;
    uint16_t three = ((*reg) & THREEMASK) >> 3;
    uint16_t five = ((*reg) & FIVEMASK) >> 5;

    uint16_t MSB = (((zero ^ two) ^ three) ^ five) << 15;
    *reg = (((*reg) >> 1) & (~(0x0001 << 15))) | MSB;
}
