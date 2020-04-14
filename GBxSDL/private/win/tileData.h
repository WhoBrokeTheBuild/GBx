#ifndef TILE_DATA_H
#define TILE_DATA_H

#include <GBx/GBx.h>

void InitTileDataTab(gbx_t * ctx);

void TermTileDataTab(gbx_t * ctx);

void TileDataTabRefresh(gbx_t * ctx);

void TileDataTabRender(gbx_t * ctx);

#endif // TILE_DATA_H