#pragma once
#include <SDL3/SDL.h>

class Renderer
{
public:
    Renderer(SDL_Renderer* renderer);

    void DrawAnalogFrame(int cx, int cy, int radius);
    void DrawHands(int cx, int cy, int radius, int hours, int minutes, int seconds);
    void DrawDigitalFrame(int x, int y, int w, int h, int hours, int minutes, int seconds);

private:
    SDL_Renderer* renderer;
};
