#ifndef GBX_APU_H
#define GBX_APU_H

#include <GBx/GBx.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GBX_WAVE_RAM_SIZE (0x10)

// Audio Processing Unit
struct GBx_APU
{
    // 0.0 - 1.0
    float Volume;

    uint8_t WaveRAM[GBX_WAVE_RAM_SIZE];

};

typedef struct GBx_APU GBx_APU;

GBx_APU * GBx_GetAPU(GBx * ctx);

void GBx_APU_Reset(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_APU_H