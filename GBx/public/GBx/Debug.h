#ifndef GBX_DEBUG_H
#define GBX_DEBUG_H

#include <GBx/Types/Debug.h>

typedef struct gbx gbx_t;

void GBx_InitDebug(gbx_t * ctx);

void GBx_TermDebug(gbx_t * ctx);

void GBx_InitMemoryTracker(gbx_t * ctx);

void GBx_TermMemoryTracker(gbx_t * ctx);

void GBx_AgeMemoryTracker(gbx_t * ctx);

#endif // GBX_DEBUG_H