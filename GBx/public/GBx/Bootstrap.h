#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include <GBx/Types.h>

typedef struct gbx gbx_t;

bool GBx_LoadBootstrap(gbx_t * ctx, const char * filename);

void GBx_StubBootstrap(gbx_t * ctx);

#endif // BOOTSTRAP_H