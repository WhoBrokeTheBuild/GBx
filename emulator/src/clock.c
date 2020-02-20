#include "clock.h"

#include "apu.h"
#include "lcd.h"
#include "timer.h"

uint64_t Ticks = 0;

unsigned ClockSpeed = GB_CLOCK_SPEED;

void tick(unsigned cycles)
{
    timerTick(cycles);
    lcdTick(cycles);
    apuTick(cycles);
}