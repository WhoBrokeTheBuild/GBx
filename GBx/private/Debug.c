#include <GBx/Debug.h>

#include <string.h>

#include "Internal.h"
#include "Log.h"

#define GBX_MEMORY_TRACKER_SIZE (0x10000)

void GBx_InitMemoryTracker(gbx_t * ctx)
{
    ctx->internal->MemoryTracker = 
        malloc(sizeof(gbx_memory_tracker_entry_t) * GBX_MEMORY_TRACKER_SIZE);
}

void GBx_TermMemoryTracker(gbx_t * ctx)
{
    free(ctx->internal->MemoryTracker);
    ctx->internal->MemoryTracker = NULL;
}

void GBx_AgeMemoryTracker(gbx_t * ctx)
{
    gbx_internal_t * inctx = ctx->internal;

    if (!inctx->MemoryTracker) {
        return;
    }

    for (unsigned i = 0; i < sizeof(GBX_MEMORY_TRACKER_SIZE); ++i) {
        if (inctx->MemoryTracker[i].Read > 0) {
            --inctx->MemoryTracker[i].Read;
        }
        if (inctx->MemoryTracker[i].Write > 0) {
            --inctx->MemoryTracker[i].Write;
        }
    }
}