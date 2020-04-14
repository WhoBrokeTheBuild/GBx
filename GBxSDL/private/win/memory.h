#ifndef WIN_MEMORY_H
#define WIN_MEMORY_H

#include <GBx/GBx.h>

void InitMemoryTab(gbx_t * ctx);

void TermMemoryTab(gbx_t * ctx);

void MemoryTabRefresh(gbx_t * ctx);

void MemoryTabRender(gbx_t * ctx);

#endif // WIN_MEMORY_H