#include "Game.h"
#include <iostream>
#include <cmath> // Pour sin()

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

Game::Game() : audioDevice(0), audioStream(nullptr) {}

Game::~Game() {
    CleanUp();
}

bool Game::Init() {
    // 1. Initialiser SDL avec Vidéo et Audio
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cerr << "Erreur SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

    // 2. Création de la fenêtre et du renderer
    window = SDL_CreateWindow("Horloge Analogique Digital", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) return false;

    // 3. Configuration de l'Audio pour SDL3
    // Dans Game::Init()
    SDL_AudioSpec spec;
    spec.format = SDL_AUDIO_F32;
    spec.channels = 1;
    spec.freq = 44100;

    // Correction du nom de la constante pour SDL3
    audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    
    if (audioDevice == 0) {
        std::cerr << "Erreur Audio Device: " << SDL_GetError() << std::endl;
    } else {
        audioStream = SDL_CreateAudioStream(&spec, &spec);
        if (audioStream) {
            SDL_BindAudioStream(audioDevice, audioStream);
        }
    }

    ui = new UI(window, renderer);
    clockRenderer = new Renderer(renderer);
    running = true;
    return true;
}

void Game::Run() {
    if (!Init()) return;

    while (running) {
        HandleEvents();
        timeManager.Update();

        int h = timeManager.GetHours();
        int m = timeManager.GetMinutes();
        int s = timeManager.GetSeconds();

        // --- LOGIQUE DE L'ALARME ---
        if (myAlarm.active && !myAlarm.ringing) {
            if (h == myAlarm.hour && m == myAlarm.minute && s == 0) {
                myAlarm.ringing = true;
                if (audioDevice) SDL_ResumeAudioDevice(audioDevice);
            }
        }

        // --- GÉNÉRATION DU SON (BIP) ---
        if (myAlarm.ringing && audioStream) {
            // On génère un petit tampon de son (onde sinusoïdale)
            float buffer[512];
            static double phase = 0;
            for (int i = 0; i < 512; i++) {
                buffer[i] = 0.2f * sin(phase); // Volume à 20%
                phase += 0.15; // Fréquence du bip
            }
            // On envoie les données mathématiques au flux audio
            SDL_PutAudioStreamData(audioStream, buffer, sizeof(buffer));
        } 
        else if (!myAlarm.ringing && audioDevice) {
            SDL_PauseAudioDevice(audioDevice);
            SDL_ClearAudioStream(audioStream); // On vide le flux pour arrêter le son net
        }

        Render();
        SDL_Delay(1);
    }
    CleanUp();
}

void Game::HandleEvents() {
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

void Game::Render() {
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
}

void Game::CleanUp() {
    if (audioStream) SDL_DestroyAudioStream(audioStream);
    if (ui) { delete ui; ui = nullptr; }
    if (clockRenderer) delete clockRenderer;
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}