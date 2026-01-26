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
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cerr << "Erreur SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Horloge Analogique Digital", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) return false;

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
        
        // --- MISE À JOUR DU TEMPS ET LOGIQUE ---
        timeManager.Update();

        // LOGIQUE DE L'ALARME : On vérifie si ça doit sonner
        int h = timeManager.GetHours();
        int m = timeManager.GetMinutes();
        int s = timeManager.GetSeconds();

        if (myAlarm.active && !myAlarm.ringing) {
            //on vérifie l'heure et la minute
            // Déclenchement si l'heure correspond (à la seconde 0 pour éviter les doublons)
            // On ajoute un flag interne pour éviter que ça ne se déclenche 60 fois par seconde
            static int lastTriggerMinute = -1;
            bool isMainTime = (h == myAlarm.hour && m == myAlarm.minute && s == 0);
            bool isSnoozeTime = (myAlarm.snoozeActive && h == myAlarm.snoozeHour && m == myAlarm.snoozeMinute && s == 0);

            if ((isMainTime || isSnoozeTime) && lastTriggerMinute != m) {
                myAlarm.ringing = true;
                myAlarm.snoozeActive = false; // Désactive le mode snooze s'il était actif
                lastTriggerMinute = m; // On mémorise qu'on a déjà fait sonner cette minute
                std::cout << "Tok TOK Tok ! Il est " << h << ":" << m << std::endl;
            }

            // Reset du flag quand la minute passe pour permettre à l'alarme de demain de sonner
            if (m != myAlarm.minute && !myAlarm.snoozeActive) {
                lastTriggerMinute = -1;
            }

            if ( myAlarm.ringing ) {
                //bip system comme son d'alarm
                static int lastBipSecond = -1;
                if ( s != lastBipSecond ) {
                    std::cout <<"\a" << std::flush; //bip sonore
                }
            }
        }
        // ---------------------------------------

        Render();
        SDL_Delay(1); 
    }
    CleanUp();
}

void Game::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (ui) ui->ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT) running = false;
        if (ImGui::GetIO().WantCaptureMouse) continue;

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_F) use24hFormat = !use24hFormat;
        }
    }
}

void Game::Render()
{
    if (ui) {
        ui->BeginFrame();
        ui->RenderInterface(use24hFormat, currentTheme, showAnalog, showDigital, 
                            analogScale, digitalScale, showDemo, myAlarm);
    }

    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);
    
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

    if (ui) ui->EndFrame();
    SDL_RenderPresent(renderer);

    if (myAlarm.ringing) {
        //bip pour alarm
        std::cout << '\a'; //le carractere 'bell' genere un "bip" du systeme
    }
}

void Game::CleanUp()
{
    if (ui) { delete ui; ui = nullptr; }
    if (clockRenderer) delete clockRenderer;
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}