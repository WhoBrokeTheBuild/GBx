#ifndef GBX_TIMER_H
#define GBX_TIMER_H

#include <GBx/GBx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union GBx_TimerControl
{
    GBX_PACK(struct {
        uint8_t Type:2;
        uint8_t Enabled:1;
        uint8_t :5;
    });

    uint8_t _raw;

} GBx_TimerControl;

static_assert(
    sizeof(GBx_TimerControl) == 1,
    "sizeof(GBx_TimerControl) != 1"
);

void GBx_Timer_Reset(GBx * ctx);

void GBx_Timer_Tick(GBx * ctx, unsigned cycles);

#ifdef __cplusplus
}
#endif

#endif // GBX_TIMER_H