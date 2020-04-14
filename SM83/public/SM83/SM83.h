#ifndef SM83_H
#define SM83_H

#include <SM83/CPU.h>
#include <SM83/Context.h>
#include <SM83/Debug.h>
#include <SM83/Disassembler.h>
#include <SM83/Interrupts.h>
#include <SM83/Types.h>

sm83_t * SM83_Init();

void SM83_Term(sm83_t * ctx);

void SM83_Reset(sm83_t * ctx);

bool SM83_GetEnabled(sm83_t * ctx);

void SM83_SetEnabled(sm83_t * ctx, bool enabled);

int SM83_GetVerboseLevel(sm83_t * ctx);

void SM83_SetVerboseLevel(sm83_t * ctx, int level);

sm83_mode_t SM83_GetMode(sm83_t * ctx);

void SM83_SetMode(sm83_t * ctx, sm83_mode_t mode);

int SM83_GetClockSpeed(sm83_t * ctx);

uintmax_t SM83_GetTotalTicks(sm83_t * ctx);

#endif // SM83_H