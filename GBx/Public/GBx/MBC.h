#ifndef GBX_MBC_H
#define GBX_MBC_H

#include <GBx/GBx.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum GBx_MemoryBankControlType
{
    GBX_MBC_TYPE_NONE,
    GBX_MBC_TYPE_MBC1,
    GBX_MBC_TYPE_MBC2,
    GBX_MBC_TYPE_MBC3,
    GBX_MBC_TYPE_MBC5,
    GBX_MBC_TYPE_MBC6,
    GBX_MBC_TYPE_MBC7,
    GBX_MBC_TYPE_MMM01,

} GBx_MemoryBankControlType;

typedef union GBx_MemoryBankControl
{
    GBX_PACK(struct {
        uint8_t Lower:5;
        uint8_t Upper:2;
        uint8_t :1;
    });

    GBX_PACK(struct {
        uint8_t Full:7;
        uint8_t Mode:1;
    });

    uint8_t _raw;

} GBx_MemoryBankControl;

static_assert(
    sizeof(GBx_MemoryBankControl) == 1,
    "sizeof(GBx_MemoryBankControl) != 1"
);

typedef union GBX_RealTimeClock
{
    GBX_PACK(struct {
        // Seconds, 0-59
        uint8_t S;

        // Minutes, 0-59
        uint8_t M;

        // Hours, 0-23
        uint8_t H;

        union {
            struct {
                uint8_t DL;
                uint8_t DH;
            };

            struct {
                // 0-511
                uint16_t DayCounter:9;

                uint8_t :5;

                // 0 = Active, 1 = Stopped
                uint8_t Halt:1;

                // Set when Day Counter overflows
                uint8_t DayCounterCarry:1;
            };
        };
    });

    uint8_t _raw[5];

} GBx_RealTimeClock;

static_assert(
    sizeof(GBx_RealTimeClock) == 5,
    "sizeof(GBx_RealTimeClock) != 5"
);

void GBx_MBC_Reset(GBx * ctx);

void GBx_MBC_WriteByte(GBx * ctx, uint16_t address, uint8_t data);

const char * GBx_MBC_GetTypeString(uint8_t type);

#ifdef __cplusplus
}
#endif

#endif // GBX_MBC_H