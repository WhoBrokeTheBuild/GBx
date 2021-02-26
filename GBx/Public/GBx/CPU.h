#ifndef GBX_CPU_H
#define GBX_CPU_H

#include <GBx/GBx.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

union GBx_InterruptFlags
{
    GBX_PACK(struct {
        uint8_t Int40:1;
        uint8_t Int48:1;
        uint8_t Int50:1;
        uint8_t Int58:1;
        uint8_t Int60:1;
        uint8_t :3;
    });

    uint8_t _raw;

};

typedef union GBx_InterruptFlags GBx_InterruptFlags;

static_assert(
    sizeof(GBx_InterruptFlags) == 1,
    "sizeof(GBx_InterruptFlags) != 1"
);

#define GBX_WRAM_BANK_COUNT (8)
#define GBX_WRAM_BANK_SIZE  (0x1000)

#define GBX_HRAM_SIZE (0x7F)

struct GBx_CPU
{
    bool Enabled;

    // Work RAM
    uint8_t WRAM[GBX_WRAM_BANK_COUNT][GBX_WRAM_BANK_SIZE];

    unsigned WRAMBank;

    // High RAM / Zero Page
    uint8_t HRAM[GBX_HRAM_SIZE];

    union {
        struct {
            union {
                GBX_PACK(struct {
                    uint8_t :4;
                    uint8_t FC:1; // Carry Flag
                    uint8_t FH:1; // Half-Carry Flag
                    uint8_t FN:1; // Subtract Flag
                    uint8_t FZ:1; // Zero Flag
                });

                uint8_t F;
            };

            uint8_t A;
        };

        uint16_t AF;
    };

    union {
        struct {
            uint8_t C;
            uint8_t B;
        };

        uint16_t BC;
    };

    union {
        struct {
            uint8_t E;
            uint8_t D;
        };

        uint16_t DE;
    };

    union {
        struct {
            uint8_t L;
            uint8_t H;
        };

        uint16_t HL;
    };

    uint16_t SP;
    uint16_t PC;

    bool IME;
    bool RequestEnableIME;

    GBx_InterruptFlags IF;
    GBx_InterruptFlags IE;

};

typedef struct GBx_CPU GBx_CPU;

GBx_CPU * GBx_GetCPU(GBx * ctx);

void GBx_CPU_Reset(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_CPU_H