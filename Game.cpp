#include "Game.h"
#include <iostream>
#include <cmath> // Pour sin()

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

Game::Game() : audioDevice(0), audioStream(nullptr) {}

// Fonction simple pour convertir l'arc-en-ciel (HSV) en couleurs utilisables par SDL (RGB)
void HSVtoRGB(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b) {
    float f = h * 6.0f;
    int i = (int)f;
    float f_part = f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f_part);
    float t = v * (1.0f - s * (1.0f - f_part));
    float rv, gv, bv;
    switch (i % 6) {
        case 0: rv = v; gv = t; bv = p; break;
        case 1: rv = q; gv = v; bv = p; break;
        case 2: rv = p; gv = v; bv = t; break;
        case 3: rv = p; gv = q; bv = v; break;
        case 4: rv = t; gv = p; bv = v; break;
        case 5: rv = v; gv = p; bv = q; break;
    }
    r = (uint8_t)(rv * 255);
    g = (uint8_t)(gv * 255);
    b = (uint8_t)(bv * 255);
}

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
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    int cx = w / 2;
    int cy = h / 2;
    int dynamicRadius = (w < h ? w : h) * 0.30f * analogScale;

    // --- 1. CALCUL ET DESSIN DU FOND ARC-EN-CIEL (SDL3) ---
    static float hue = 0.0f;
    hue += 0.0005f; 
    if (hue > 1.0f) hue = 0.0f;

    uint8_t r, g, b;
    // Saturation 0.4, Valeur 0.15 (sombre pour que les aiguilles ressortent)
    HSVtoRGB(hue, 0.4f, 0.15f, r, g, b); 

    // On remplit tout l'arrière-plan SDL avec cette couleur
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);

    // --- 2. DESSIN DES HORLOGES (SDL3) ---
    // Puisque le fond est fait, on dessine par-dessus
    if (clockRenderer) {
        if (showAnalog) {
            clockRenderer->DrawAnalogFrame(cx, cy, dynamicRadius);
            clockRenderer->DrawHands(cx, cy, dynamicRadius,
                                    timeManager.GetHours(), timeManager.GetMinutes(), timeManager.GetSeconds());
        }
        
        if (showDigital) {
            int digW = 300 * digitalScale;
            int digH = 60 * digitalScale;
            clockRenderer->DrawDigitalDisplay(cx - (digW / 2), cy + dynamicRadius + 20, digW, digH,
                                             timeManager.GetHours(), timeManager.GetMinutes(), timeManager.GetSeconds());
        }
    }

    // --- 3. DESSIN DE L'INTERFACE (ImGui) ---
    // ImGui est toujours dessiné en dernier pour être au premier plan
    if (ui) {
        ui->BeginFrame();
        ui->RenderInterface(use24hFormat, currentTheme, showAnalog, showDigital, 
                            analogScale, digitalScale, showDemo, myAlarm);
        ui->EndFrame();
    }

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