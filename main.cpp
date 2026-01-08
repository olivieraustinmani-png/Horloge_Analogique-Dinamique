#define SDL_MAIN_HANDLED
#include "Game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
/*#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"*/

int main(int argc, char* argv[])
{
    Game game;
    game.Run();
    return 0;
}