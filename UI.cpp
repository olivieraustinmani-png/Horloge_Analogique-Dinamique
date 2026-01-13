#include "UI.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <iostream>

UI::UI(SDL_Window* win, SDL_Renderer* rend)
    : window(win), renderer(rend)
{
    Initialize();
}

UI::~UI()
{
    // Nettoyage impératif des backends avant de détruire le contexte
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    if (imguiContext) {
        ImGui::DestroyContext(imguiContext);
        imguiContext = nullptr;
    }
}

bool UI::Initialize()
{
    IMGUI_CHECKVERSION();
    imguiContext = ImGui::CreateContext();
    if (!imguiContext) return false;

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    
    // Initialisation des ponts SDL3 et Renderer
    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        return false;
    }
    
    if (!ImGui_ImplSDLRenderer3_Init(renderer)) {
        return false;
    }
    
    return true;
}

void UI::ProcessEvent(const SDL_Event* event)
{
    ImGui_ImplSDL3_ProcessEvent(event);
}

void UI::BeginFrame()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void UI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void UI::RenderInterface(bool& use24hFormat, int& currentTheme, 
                        bool& showAnalog, bool& showDigital,
                        float& analogScale, float& digitalScale,
                        bool& showDemo)
{
    RenderMainMenuBar();
    RenderClockControls(use24hFormat, currentTheme, showAnalog, showDigital,
                       analogScale, digitalScale, showDemo);
    RenderThemePreview(currentTheme);
    
    if (showDemo) {
        ImGui::ShowDemoWindow(&showDemo);
    }
}

void UI::RenderMainMenuBar()
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Fichier")) {
            if (ImGui::MenuItem("Quitter", "Alt+F4")) {
                SDL_Event quitEvent;
                quitEvent.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quitEvent);
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Aide")) {
            if (ImGui::MenuItem("A propos")) {
                ImGui::OpenPopup("AboutPopup");
            }
            ImGui::EndMenu();
        }

        // Gestion du Pop-up "A propos"
        if (ImGui::BeginPopupModal("AboutPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Horloge Analogique-Digitale v1.0");
            ImGui::Separator();
            ImGui::Text("Developpe avec SDL3 et ImGui.");
            if (ImGui::Button("Fermer", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void UI::RenderClockControls(bool& use24hFormat, int& currentTheme, 
                            bool& showAnalog, bool& showDigital,
                            float& analogScale, float& digitalScale,
                            bool& showDemo)
{
    ImGui::Begin("Reglages de l'Horloge", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::CollapsingHeader("Affichage", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Analogique", &showAnalog);
        ImGui::Checkbox("Digitale", &showDigital);
        
        ImGui::Separator();
        ImGui::Text("Format:");
        ImGui::RadioButton("24h", (int*)&use24hFormat, 1); ImGui::SameLine();
        ImGui::RadioButton("12h", (int*)&use24hFormat, 0);
    }

    if (ImGui::CollapsingHeader("Tailles")) {
        ImGui::SliderFloat("Echelle Analogique", &analogScale, 0.5f, 2.0f);
        ImGui::SliderFloat("Echelle Digitale", &digitalScale, 0.5f, 2.0f);
    }

    if (ImGui::CollapsingHeader("Themes")) {
        const char* themes[] = { "Classique", "Nuit", "Retro", "Minimal" };
        ImGui::Combo("Selection Thème", &currentTheme, themes, IM_ARRAYSIZE(themes));
    }

    ImGui::Separator();
    ImGui::Checkbox("Debug Demo ImGui", &showDemo);
    
    ImGui::End();
}

void UI::RenderThemePreview(int currentTheme)
{
    ImGui::Begin("Apercu Theme");
    ImGui::Text("Theme selectionne : %d", currentTheme);
    // Ici tu pourras ajouter des pastilles de couleur plus tard
    ImGui::End();
}