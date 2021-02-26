#ifndef GBX_BOOTSTRAP_H
#define GBX_BOOTSTRAP_H

#include <GBx/GBx.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct GBx_Bootstrap
{
    bool Enabled;

    uint8_t ROM[0x900];

};

typedef struct GBx_Bootstrap GBx_Bootstrap;

GBx_Bootstrap * GBx_GetBootstrap(GBx * ctx);

void GBx_Bootstrap_Reset(GBx * ctx);

bool GBx_Bootstrap_Load(GBx * ctx, const char * filename);

#ifdef __cplusplus
}
#endif

#endif // GBX_BOOTSTRAP_H