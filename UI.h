#pragma once
#include <SDL3/SDL.h>

/*#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_sdl3.h"
#include "../imgui/backends/imgui_impl_sdlrenderer3.h"

// Forward declaration ImGui
struct ImGuiContext;

class UI
{
public:
    UI(SDL_Window* window, SDL_Renderer* renderer);
    ~UI();

    // Initialisation ImGui
    bool Initialize();
    
    // Gestion des frames ImGui
    void BeginFrame();
    void EndFrame();
    
    // Rendu de l'interface
    void RenderInterface(bool& use24hFormat, int& currentTheme, 
                        bool& showAnalog, bool& showDigital,
                        float& analogScale, float& digitalScale,
                        bool& showDemo);

    // Gestion des événements (à appeler avant SDL)
    void ProcessEvent(const SDL_Event* event);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    ImGuiContext* imguiContext = nullptr;
    
    // Méthodes de rendu internes
    void RenderMainMenuBar();
    void RenderClockControls(bool& use24hFormat, int& currentTheme, 
                            bool& showAnalog, bool& showDigital,
                            float& analogScale, float& digitalScale,
                            bool& showDemo);
    void RenderThemePreview( int currentTheme);
};*/