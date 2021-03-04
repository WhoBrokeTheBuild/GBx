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



struct GBx_CPU
{
    bool Enabled;

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

    // Debug


};

typedef struct GBx_CPU GBx_CPU;

GBx_CPU * GBx_GetCPU(GBx * ctx);

void GBx_CPU_Reset(GBx * ctx);

void GBx_CPU_Step(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_CPU_H