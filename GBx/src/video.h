#ifndef VIDEO_H
#define VIDEO_H

void VideoInit();
void VideoTerm();

void PollEvents();

void Render();

void DebugWindowInit();
void DebugWindowTerm();

void DebugWindowRender();

#endif // VIDEO_H