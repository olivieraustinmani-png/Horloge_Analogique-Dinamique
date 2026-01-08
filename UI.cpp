#include "UI.h"

// Includes ImGui (adaptés à ta structure)
/*#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_sdlrenderer3.h"

#include <iostream>

UI::UI(SDL_Window* win, SDL_Renderer* rend)
    : window(win), renderer(rend)
{
    Initialize();
}

UI::~UI()
{
    if (imguiContext) {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext(imguiContext);
        imguiContext = nullptr;
    }
}

bool UI::Initialize()
{
    std::cout << "Initialisation ImGui..." << std::endl;
    
    // Setup ImGui
    IMGUI_CHECKVERSION();
    imguiContext = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Style
    ImGui::StyleColorsDark();
    
    // Setup Platform/Renderer backends
    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        std::cerr << "Erreur ImGui_ImplSDL3_InitForSDLRenderer" << std::endl;
        return false;
    }
    
    if (!ImGui_ImplSDLRenderer3_Init(renderer)) {
        std::cerr << "Erreur ImGui_ImplSDLRenderer3_Init" << std::endl;
        return false;
    }
    
    std::cout << "ImGui initialise avec succes!" << std::endl;
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
    //RenderThemePreview(int currentTheme);
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
        
        if (ImGui::BeginMenu("Affichage")) {
            ImGui::MenuItem("Montres", NULL, false, false);
            ImGui::Separator();
            ImGui::MenuItem("Analogique", NULL, false, false);
            ImGui::MenuItem("Digitale", NULL, false, false);
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Aide")) {
            if (ImGui::MenuItem("A propos")) {
                // Pourrait ouvrir une autre fenêtre
                ImGui::OpenPopup("A propos");
            }
            ImGui::EndMenu();
        }
        
        // Fenêtre "A propos" si ouverte
        if (ImGui::BeginPopupModal("A propos", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Horloge Analogique-Digitale");
            ImGui::Text("Developpe avec SDL3 et ImGui");
            ImGui::Separator();
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
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
    ImGui::Begin("Controles Horloge", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    // Format heure
    ImGui::Text("Format heure:");
    ImGui::SameLine();
    if (ImGui::RadioButton("24h", use24hFormat)) use24hFormat = true;
    ImGui::SameLine();
    if (ImGui::RadioButton("12h", !use24hFormat)) use24hFormat = false;
    
    ImGui::Separator();
    
    // Affichage
    ImGui::Text("Affichage:");
    ImGui::Checkbox("Montre Analogique", &showAnalog);
    ImGui::Checkbox("Montre Digitale", &showDigital);
    
    ImGui::Separator();
    
    // Échelle
    ImGui::Text("Taille:");
    ImGui::SliderFloat("Analogique", &analogScale, 0.5f, 2.0f, "%.1f");
    ImGui::SliderFloat("Digitale", &digitalScale, 0.5f, 2.0f, "%.1f");
    
    ImGui::Separator();
    
    // Thème
    ImGui::Text("Theme:");
    const char* themes[] = { "Classique", "Nuit", "Retro", "Minimal" };
    ImGui::Combo("Selection", &currentTheme, themes, IM_ARRAYSIZE(themes));
    
    ImGui::Separator();
    
    // Demo ImGui
    ImGui::Checkbox("Montrer demo ImGui", &showDemo);
    
    ImGui::End();
}

void UI::RenderThemePreview( int currentTheme)
{
    ImGui::Begin("Apercu Theme", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Couleurs actuelles:");
    ImGui::Separator();
    
    // Couleurs des thèmes (exemple)
    switch (currentTheme) {
        case 0: // Classique
            ImGui::ColorButton("Fond", ImVec4(0.1f, 0.1f, 0.15f, 1.0f));
            ImGui::SameLine(); ImGui::Text("Fond sombre");
            break;
        case 1: // Nuit
            ImGui::ColorButton("Fond", ImVec4(0.05f, 0.05f, 0.1f, 1.0f));
            ImGui::SameLine(); ImGui::Text("Bleu nuit");
            break;
        case 2: // Retro
            ImGui::ColorButton("Digital", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::SameLine(); ImGui::Text("Vert retro");
            break;
    }
    
    ImGui::End();
}*/