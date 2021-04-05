#ifndef GBX_JOYPAD_H
#define GBX_JOYPAD_H

#include <GBx/GBx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union GBx_Joypad
{
    // D-Pad
    GBX_PACK(struct {
        uint8_t Right:1;
        uint8_t Left:1;
        uint8_t Up:1;
        uint8_t Down:1;
        uint8_t :4;
    });

    // Buttons
    GBX_PACK(struct {
        uint8_t A:1;
        uint8_t B:1;
        uint8_t Select:1;
        uint8_t Start:1;
        uint8_t :4;
    });

    // Control
    GBX_PACK(struct {
        uint8_t InputStates:4;
        uint8_t SelectDPad:1;
        uint8_t SelectButtons:1;
        uint8_t :2;
    });

    uint8_t _raw;

} GBx_Joypad;

static_assert(
    sizeof(GBx_Joypad) == 1,
    "sizeof(GBx_Joypad) != 1"
);

#define GBX_JOYP_READ_MASK      0b11000000
#define GBX_JOYP_WRITE_MASK     0b00110000

void GBx_Joypad_Reset(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_JOYPAD_H