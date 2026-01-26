#pragma once
#include <SDL3/SDL.h>
#include "imgui.h"

class Renderer
{
public:

    void initImGui(SDL_Window* window, SDL_Renderer* sdlrenderer);
    void shutdownImGui();

    Renderer(SDL_Renderer* renderer);
    
    // Analogique
    void DrawAnalogFrame(int cx, int cy, int radius);
    void DrawDigit(int x, int y, int size, int digit);
    void DrawHands(int cx, int cy, int radius, int hours, int minutes, int seconds);
    
    // Digitale
    void DrawDigitalDisplay(int x, int y, int w, int h, int hours, int minutes, int seconds);

private:
    SDL_Renderer* renderer;
    
    // TES fonctions de chiffres
    void DrawColon(int x, int y, int size);
    void DrawDigitalTime(int x, int y, int size, int h, int m, int s);
};