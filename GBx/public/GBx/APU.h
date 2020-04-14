#ifndef GBX_APU_H
#define GBX_APU_H

#include <GBx/Types/APU.h>

// extern uint8_t DutyCycles[4][8];

typedef struct gbx gbx_t;

void GBx_ResetAPU(gbx_t * ctx);

void GBx_InitAPU(gbx_t * ctx);
void GBx_TermAPU(gbx_t * ctx);

void GBx_TickAPU(gbx_t * ctx, unsigned cycles);

void GBx_PauseAPU(gbx_t * ctx);
void GBx_PlayAPU(gbx_t * ctx);

const char * GBx_GetVolumeCodeString(unsigned index);

void GBx_PrintAPUC(gbx_t * ctx);
void GBx_PrintVolumeControl(gbx_t * ctx);
void GBx_PrintSoundOutputTerminal(gbx_t * ctx);
void GBx_PrintTone1(gbx_t * ctx);
void GBx_PrintTone2(gbx_t * ctx);
void GBx_PrintWave(gbx_t * ctx);
void GBx_PrintNoise(gbx_t * ctx);
void GBx_PrintWaveRAM(gbx_t * ctx);

#endif // GBX_APU_H