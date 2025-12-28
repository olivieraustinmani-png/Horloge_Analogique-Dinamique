#pragma once
#include <SDL3/SDL.h>

class Renderer
{
public:
    Renderer(SDL_Renderer* renderer);

    void DrawAnalogFrame(int cx, int cy, int radius);

    void DrawHands(int cx, int cy, int radius, int hours, int minutes, int seconds);

    void DrawDigitalFrame(int x, int y, int w, int h, int hours, int minutes, int seconds);

    void DrawDigit(int x, int y, int size, int digit);
    
    void DrawTimeDigital(int x, int y, int size, int h, int m, int s);


private:
    SDL_Renderer* renderer;
};
