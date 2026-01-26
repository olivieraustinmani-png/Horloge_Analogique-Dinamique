#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include "Renderer.h"
#include "TimeManager.h"
#include "UI.h"

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

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Renderer* clockRenderer = nullptr;
    UI* ui = nullptr; // On utilise la UI dédiée
    bool running = false;

    TimeManager timeManager;
    
    // Variables de contrôle pour l'interface
    bool use24hFormat = true;
    int currentTheme = 0;
    bool showAnalog = true;
    bool showDigital = true;
    float analogScale = 1.0f;
    float digitalScale = 1.0f;
    bool showDemo = false;

    //Declaration de l'alarme
    Alarm myAlarm;
    SDL_AudioDeviceID audioDevice = 0;
    SDL_AudioStream* audioStream = nullptr;
};

#endif