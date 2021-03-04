#ifndef GBX_H
#define GBX_H

#include <GBx/Version.h>
#include <GBx/Platform.h>
#include <GBx/Macros.h>
#include <GBx/Context.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward Declaration, include <GBx/Context.h>
typedef struct GBx GBx;

struct GBx_Config
{

};

typedef struct GBx_Config GBx_Config;

bool GBx_Init(GBx ** pctx, GBx_Config * cfg);

void GBx_Term(GBx ** ctx);

void GBx_Reset(GBx * ctx);

void GBx_Tick(GBx * ctx, unsigned cycles);

void GBx_Step(GBx * ctx);

void GBx_Frame(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_H