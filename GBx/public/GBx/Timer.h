#ifndef TIMER_H
#define TIMER_H

#include <GBx/Types.h>

typedef struct gbx gbx_t;

void GBx_ResetTimer(gbx_t * ctx);

void GBx_TickTimer(gbx_t * ctx, unsigned cycles);

int GBx_GetTimerSpeed(gbx_t * ctx);

void GBx_PrintTimer(gbx_t * ctx);

#endif // TIMER_H