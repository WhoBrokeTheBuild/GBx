#ifndef GBX_INTERRUPT_H
#define GBX_INTERRUPT_H

#include <GBx/GBx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union GBx_InterruptFlags
{
    GBX_PACK(struct {
        uint8_t Int40:1; // V-Blank
        uint8_t Int48:1; // LCD STAT
        uint8_t Int50:1; // Timer
        uint8_t Int58:1; // Serial
        uint8_t Int60:1; // Joypad
        uint8_t :3;
    });

    uint8_t _raw;

} GBx_InterruptFlags;

static_assert(
    sizeof(GBx_InterruptFlags) == 1,
    "sizeof(GBx_InterruptFlags) != 1"
);

unsigned GBx_CallInterrupt(GBx * ctx, uint16_t addr);

unsigned GBx_CheckInterrupts(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_INTERRUPT_H