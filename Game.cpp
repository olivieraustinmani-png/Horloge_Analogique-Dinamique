#include "Game.h"
#include <iostream>
#include "Renderer.h"
#include "UI.h"
#include "TimeManager.h"

/*#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"*/



Game::Game() {}

Game::~Game() {
    CleanUp();
}

bool Game::Init()
{
    if (!SDL_Init(SDL_INIT_VIDEO) != 0) {
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
    if (!renderer) return false;
    
    // --- ImGui init ---
     /*   IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);*/

        // UI
        //ui = new UI(window, renderer);

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
    if (!Init()) {
        std::cerr << "Erreur d'initialisation SDL3." << std::endl;
        return;
    }

    // Création de l'UI après renderer
    //ui = new UI(window, renderer);

    // Boucle principale
    while (running)
    {
        // Gestion des événements
        HandleEvents();

        // Mise à jour des données (heure, etc.)
        timeManager.Update();

        // Démarrage de la frame ImGui
        /*ui->BeginFrame();
        ui->Render();*/
        
        // Rendu principal de l'horloge
        Render();

        // Fin de frame ImGui
        /*ui->EndFrame();*/

        // Petite pause pour limiter à ~60 FPS
        SDL_Delay(16);
    }

    // Nettoyage
    /*if (ui) { delete ui; ui = nullptr; }
    CleanUp();*/
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
    timeManager.Update();
    // Plus tard : on passera les valeurs à Renderer pour l'affichage
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    // --- ImGui new frame ---
        /*ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();*/


    if ( clockRenderer) {
        clockRenderer->DrawAnalogFrame(300, 300, 200);
        clockRenderer->DrawDigitalFrame(180, 540, 240, 60,
                                        timeManager.GetHours(),
                                        timeManager.GetMinutes(),
                                        timeManager.GetSeconds());
        clockRenderer->DrawHands(300, 300, 180,
                                timeManager.GetHours(),
                                timeManager.GetMinutes(),
                                timeManager.GetSeconds());
    }

    // --- ImGui render ---
        /*ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);*/

        clockRenderer->DrawTimeDigital(
        200, 550,
        20,
        timeManager.GetHours(),
        timeManager.GetMinutes(),
        timeManager.GetSeconds()
    );


    SDL_RenderPresent(renderer);
}

void Game::CleanUp()
{
    if (clockRenderer) delete clockRenderer;
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    /*ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();*/

    /*if (ui) delete ui;*/

    SDL_Quit();
}

