#ifndef GBX_H
#define GBX_H

#include <GBx/Version.h>
#include <GBx/Platform.h>
#include <GBx/Macros.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward Declaration, include <GBx/Context.h>
typedef struct GBx GBx;

typedef enum GBx_Model
{
    GBX_MODEL_DMG = 0, // Original Game Boy
    GBX_MODEL_MGB = 1, // Game Boy Pocket
    GBX_MODEL_SGB = 2, // Super Game Boy
    GBX_MODEL_CGB = 3, // Game Boy Color 
    
} GBx_Model;

void GBx_GetVersion(int * major, int * minor, int * patch);

const char * GBx_GetVersionString();

bool GBx_Init(GBx ** pctx);

void GBx_Term(GBx ** ctx);

void GBx_Reset(GBx * ctx);

void GBx_SetModel(GBx * ctx, GBx_Model model);

unsigned GBx_GetClockSpeed(GBx * ctx);

void GBx_EnableMemoryTracking(GBx * ctx);

unsigned GBx_Step(GBx * ctx);

void GBx_Frame(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_H