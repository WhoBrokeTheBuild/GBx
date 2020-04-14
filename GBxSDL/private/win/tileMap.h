#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <GBx/GBx.h>

void InitTileMapTab(gbx_t * ctx);

void TermTileMapTab(gbx_t * ctx);

void TileMapTabRefresh(gbx_t * ctx);

void TileMapTabRender(gbx_t * ctx);

#endif // TILE_MAP_H