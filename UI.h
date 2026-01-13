#pragma once
#include <SDL3/SDL.h>
#include "imgui.h" // Assure-toi que le chemin est bon dans ton projet

class UI
{
public:
    UI(SDL_Window* window, SDL_Renderer* renderer);
    ~UI();

    bool Initialize();
    void BeginFrame();
    void EndFrame();
    
    void RenderInterface(bool& use24hFormat, int& currentTheme, 
                        bool& showAnalog, bool& showDigital,
                        float& analogScale, float& digitalScale,
                        bool& showDemo);

    void ProcessEvent(const SDL_Event* event);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    ImGuiContext* imguiContext = nullptr;
    
    void RenderMainMenuBar();
    void RenderClockControls(bool& use24hFormat, int& currentTheme, 
                            bool& showAnalog, bool& showDigital,
                            float& analogScale, float& digitalScale,
                            bool& showDemo);
    void RenderThemePreview(int currentTheme);
};