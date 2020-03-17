#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <SDL.h>

void InitTileMapTab();
void TermTileMapTab();

void TileMapTabClick(SDL_Point * mouse);

void TileMapTabRefresh();

void TileMapTabRender(SDL_Point * mouse);

#endif // TILE_MAP_H