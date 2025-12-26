#ifndef GAME_H
#define GAME_H
#include "Renderer.h"
#include "TimeManager.h"


#include <SDL3/SDL.h>

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    bool Init();
    void HandleEvents();
    void Update();
    void Render();
    void CleanUp();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Renderer* clockRenderer = nullptr;
    bool running = false;

private:
    TimeManager timeManager;    
};

#endif