#ifndef GBX_PPU_H
#define GBX_PPU_H

#include <GBx/GBx.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GBX_VRAM_BANK_COUNT     (2)
#define GBX_VRAM_BANK_SIZE      (0x2000)

#define GBX_OAM_SIZE (0xA0)

#define GBX_SCREEN_WIDTH        (160)
#define GBX_SCREEN_HEIGHT       (144)
#define GBX_SCREEN_COMPONENTS   (3) // RGB

#define GBX_SCREEN_SIZE                                                        \
    (GBX_SCREEN_WIDTH * GBX_SCREEN_HEIGHT * GBX_SCREEN_COMPONENTS)

struct GBx_PPU
{
    // Video RAM
    uint8_t VRAM[GBX_VRAM_BANK_COUNT][GBX_VRAM_BANK_SIZE];

    unsigned VRAMBank;

    // Object Attribute Memory
    uint8_t OAM[GBX_OAM_SIZE];

    uint8_t Pixels[GBX_SCREEN_SIZE];

};

typedef struct GBx_PPU GBx_PPU;

GBx_PPU * GBx_GetPPU(GBx * ctx);

void GBx_PPU_Reset(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_PPU_H