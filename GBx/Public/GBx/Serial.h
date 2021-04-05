#ifndef GBX_SERIAL_H
#define GBX_SERIAL_H

#include <GBx/GBx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union GBx_SerialControl
{
    GBX_PACK(struct {
        uint8_t ShiftClock:1;
        uint8_t FastClockEnable:1;
        uint8_t :5;
        uint8_t TransferStart:1;
    });

    uint8_t _raw;

} GBx_SerialControl;

static_assert(
    sizeof(GBx_SerialControl) == 1,
    "sizeof(GBx_SerialControl) != 1"
);

#define GBX_SC_READ_MASK_DMG  0b01111110
#define GBX_SC_READ_MASK_CGB  0b01111100

typedef void (*GBx_SerialReadCallback)(void *, uint8_t);

typedef uint8_t (*GBx_SerialWriteCallback)(void *);

void GBx_Serial_Init(GBx * ctx);

void GBx_Serial_Reset(GBx * ctx);

void GBx_Serial_Tick(GBx * ctx, unsigned cycles);

void GBx_Serial_SetCallbacks(GBx * ctx,
    GBx_SerialReadCallback readCallback, 
    GBx_SerialWriteCallback writeCallback, 
    void * userData
);

#ifdef __cplusplus
}
#endif

#endif // GBX_SERIAL_H