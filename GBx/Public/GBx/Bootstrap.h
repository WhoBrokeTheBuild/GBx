#ifndef GBX_BOOTSTRAP_H
#define GBX_BOOTSTRAP_H

#include <GBx/GBx.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void GBx_Bootstrap_Init(GBx * ctx);

void GBx_Bootstrap_Reset(GBx * ctx);

bool GBx_Bootstrap_Load(GBx * ctx, const char * filename);

#ifdef __cplusplus
}
#endif

#endif // GBX_BOOTSTRAP_H