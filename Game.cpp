#include "Game.h"
#include <iostream>
#include "Renderer.h"


Game::Game() {}

Game::~Game() {
    CleanUp();
}

bool Game::Init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Erreur SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
        "Horloge Analogique Digital",
        800, 600,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "Erreur CreateWindow: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Erreur CreateRenderer: " << SDL_GetError() << std::endl;
        return false;
    }

    clockRenderer = new Renderer(renderer);
    running = true;
    return true;
}

void Game::Run()
{
    if (!Init()) return;

    while (running) {
        HandleEvents();
        Update();
        Render();
        SDL_Delay(16); // ~60 FPS
    }
}

void Game::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
}

void Game::Update()
{
    // Rien pour l’instant
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    if ( clockRenderer) {
        clockRenderer->DrawAnalogFrame(300, 300, 200);
        clockRenderer->DrawDigitalFrame(180, 540, 240, 60);
    }

    SDL_RenderPresent(renderer);
}

void Game::CleanUp()
{
    if (clockRenderer) delete clockRenderer;
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

