#ifndef GBX_CONTEXT_H
#define GBX_CONTEXT_H

#include <GBx/GBx.h>
#include <GBx/APU.h>
#include <GBx/CPU.h>
#include <GBx/PPU.h>
#include <GBx/Cartridge.h>
#include <GBx/Bootstrap.h>
#include <GBx/Log.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GBX_MAX_LOG_FILES 10
#define GBX_MAG_LOG_CALLBACKS 10

struct GBx
{
    int VerboseLevel;

    GBx_Mode Mode;

    GBx_APU APU;

    GBx_CPU CPU;

    GBx_PPU PPU;

    GBx_Cartridge Cartridge;

    GBx_Bootstrap Bootstrap;

};

typedef struct GBx GBx;

#ifdef __cplusplus
}
#endif

#endif // GBX_CONTEXT_H