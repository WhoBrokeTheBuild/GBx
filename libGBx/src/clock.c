#include "clock.h"

#include "apu.h"
#include "lcd.h"
#include "timer.h"

#include <time.h>

uint64_t TotalTicks = 0;

// unsigned ticksSinceLastSleep = 0;

// #define TICKS_PER_SLEEP (1024 * 16)

unsigned ClockSpeed = GB_CLOCK_SPEED;

// #define NS_PER_TICK (1000000000.0 / ClockSpeed)

// struct timespec diffTimespec(struct timespec start, struct timespec end)
// {
// 	struct timespec temp;
// 	if ((end.tv_nsec-start.tv_nsec)<0) {
// 		temp.tv_sec = end.tv_sec-start.tv_sec-1;
// 		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
// 	} else {
// 		temp.tv_sec = end.tv_sec-start.tv_sec;
// 		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
// 	}
// 	return temp;
// }

void tick(unsigned ticks)
{
    timerTick(ticks);
    lcdTick(ticks);
    apuTick(ticks);
    TotalTicks += ticks;
    
    // ticksSinceLastSleep += ticks;
    // if (ticksSinceLastSleep > TICKS_PER_SLEEP) {
    //     struct timespec now;
    //     clock_gettime(CLOCK_MONOTONIC, &now);
        
    //     struct timespec end = now;
    //     end.tv_nsec += NS_PER_TICK * ticksSinceLastSleep;
    //     if (end.tv_nsec > 1000000000) {
    //         ++end.tv_sec;
    //         end.tv_nsec -= 1000000000;
    //     }
        
    //     for (;;) {
    //         if (now.tv_sec > end.tv_sec || 
    //             (now.tv_sec == end.tv_sec && now.tv_nsec >= end.tv_nsec)) {
    //             break;
    //         }
            
    //         clock_gettime(CLOCK_MONOTONIC, &now);
    //     }
        
    //     ticksSinceLastSleep = 0;
    // }
    
    // ticksSinceLastSleep += ticks;
    // if (ticksSinceLastSleep >= TICKS_PER_SLEEP) {
    //     static struct timespec lastSleep;
        
    //     struct timespec now;
    //     clock_gettime(CLOCK_MONOTONIC, &now);
    //     struct timespec delta = diffTimespec(lastSleep, now);
        
    //     long expected = (1.0 / ClockSpeed) * ticksSinceLastSleep * 1000000000;
        
    //     struct timespec wait;
    //     wait.tv_sec = 0;
    //     wait.tv_nsec = expected - delta.tv_nsec;
        
    //     // if (delta.tv_nsec < expected) {
    //     //     struct timespec wait;
    //     //     wait.tv_sec = 0;
    //     //     wait.tv_nsec = expected - delta.tv_nsec;
    //     //     // LogInfo("Sleeping for %ld", wait.tv_nsec);
    //     //     clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, NULL);
    //     // }
        
    //     clock_gettime(CLOCK_MONOTONIC, &lastSleep);
    //     ticksSinceLastSleep = 0;
    // }
}