#ifndef GBX_H
#define GBX_H

#include <GBx/APU.h>
#include <GBx/Bootstrap.h>
#include <GBx/Cartridge.h>
#include <GBx/Joypad.h>
#include <GBx/Debug.h>
#include <GBx/MMU.h>
#include <GBx/PPU.h>
#include <GBx/Serial.h>
#include <GBx/Timer.h>
#include <GBx/Context.h>

gbx_t * GBx_Init();

void GBx_Term(gbx_t * ctx);

void GBx_Tick(gbx_t * ctx, unsigned cycles);

void GBx_Reset(gbx_t * ctx);

#endif // GBX_H