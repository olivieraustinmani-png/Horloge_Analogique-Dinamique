#include "Game.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

Game::Game() {}

Game::~Game() {
    CleanUp();
}

bool Game::Init()
{
    // Correction : Utilisation d'une comparaison simple
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Erreur SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Horloge Analogique Digital", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) return false;

    // On initialise l'UI qui s'occupe de tout l'interne ImGui
    ui = new UI(window, renderer);
    
    clockRenderer = new Renderer(renderer);
    running = true;
    return true;
}

void Game::Run()
{
    if (!Init()) return;

    while (running) {
        HandleEvents();
        timeManager.Update();
        Render();
        SDL_Delay(1); // Plus fluide que 16ms pour ImGui
    }
    CleanUp();
}

void Game::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // L'UI traite l'événement en premier
        if (ui) ui->ProcessEvent(&event);

        if (event.type == SDL_EVENT_QUIT) running = false;

        // Bloquer les événements si la souris est sur une fenêtre ImGui
        if (ImGui::GetIO().WantCaptureMouse) continue;

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_F) use24hFormat = !use24hFormat;
        }
    }
}

void Game::Render()
{
    // 1. Préparer la frame ImGui via ta classe UI
    if (ui) {
        ui->BeginFrame();
        ui->RenderInterface(use24hFormat, currentTheme, showAnalog, showDigital, 
                            analogScale, digitalScale, showDemo);
    }

    // 2. Fond SDL3
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);
    
    // 3. Dessin des horloges (si activées dans l'UI)
    if (clockRenderer) {
        if (showAnalog) {
            clockRenderer->DrawAnalogFrame(400, 250, 150 * analogScale);
            clockRenderer->DrawHands(400, 250, (150 * analogScale) - 20,
                                    timeManager.GetHours(), timeManager.GetMinutes(), timeManager.GetSeconds());
        }
        
        if (showDigital) {
            clockRenderer->DrawDigitalDisplay(250, 450, 300 * digitalScale, 60 * digitalScale,
                                             timeManager.GetHours(), timeManager.GetMinutes(), timeManager.GetSeconds());
        }
    }

    // 4. Finaliser et afficher ImGui par dessus
    if (ui) ui->EndFrame();
    
    SDL_RenderPresent(renderer);
}

void Game::CleanUp()
{
    if (ui) { delete ui; ui = nullptr; }
    if (clockRenderer) delete clockRenderer;
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

