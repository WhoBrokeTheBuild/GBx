#ifndef VIDEO_H
#define VIDEO_H

#include <SDL.h>
#include <GBx/GBx.h>

void VideoInit(gbx_t * ctx, int scale);
void VideoTerm(gbx_t * ctx);

void PollEvents(gbx_t * ctx);

void HandleEvent(gbx_t * ctx, SDL_Event * evt);

void Render(gbx_t * ctx);

#endif // VIDEO_H