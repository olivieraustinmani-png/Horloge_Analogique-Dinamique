#include "Game.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

int main(int argc, char* argv[])
{
    Game game;
    game.Run();
    return 0;
}