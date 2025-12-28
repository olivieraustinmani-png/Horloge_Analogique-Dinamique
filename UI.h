/*#pragma once
#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

class UI
{
public:
    UI(SDL_Window* window, SDL_Renderer* renderer);
    ~UI();

    void BeginFrame();
    void Render();
    void EndFrame();

    // Exemple d’état d’interface
    bool showAlarm = false;
    bool is24hFormat = true;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};*/
