#ifndef VIDEO_H
#define VIDEO_H

#include <SDL.h>

void VideoInit();
void VideoTerm();

void PollEvents();

void HandleEvent(SDL_Event * evt);
void Render();

#endif // VIDEO_H