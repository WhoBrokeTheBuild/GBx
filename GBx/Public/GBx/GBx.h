#ifndef GBX_H
#define GBX_H

#include <GBx/Version.h>
#include <GBx/StatusCodes.h>
#include <GBx/Platform.h>
#include <GBx/Macros.h>

#ifdef __cplusplus
extern "C" {
#endif

// Defined in Context.h
typedef struct GBx GBx;

enum GBx_Mode
{
    GBX_MODE_DMG, // Original GameBoy
    GBX_MODE_SGB, // Super GameBoy
    GBX_MODE_CGB, // Color GameBoy
    
};

typedef enum GBx_Mode GBx_Mode;

struct GBx_Config
{

};

typedef struct GBx_Config GBx_Config;

int GBx_Init(GBx ** pctx, GBx_Config * cfg);

void GBx_Term(GBx ** ctx);

void GBx_Tick(GBx * ctx, unsigned cycles);

void GBx_Step(GBx * ctx);

void GBx_Frame(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_H