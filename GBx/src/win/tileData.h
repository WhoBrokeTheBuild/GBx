#ifndef TILE_DATA_H
#define TILE_DATA_H

#include <SDL.h>

void InitTileDataTab();
void TermTileDataTab();

void TileDataTabClick(SDL_Point * mouse);

void TileDataTabRefresh();

void TileDataTabRender(SDL_Point * mouse);

#endif // TILE_DATA_H