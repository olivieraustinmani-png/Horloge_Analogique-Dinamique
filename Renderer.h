#pragma once
#include <SDL3/SDL.h>

class Renderer
{
public:
    Renderer(SDL_Renderer* renderer);

    void DrawAnalogFrame(int cx, int cy, int radius);
    void DrawDigitalFrame(int x, int y, int w, int h);

private:
    SDL_Renderer* renderer;
};

void DrawDigitalFrame(int x, int y, int w, int h);

