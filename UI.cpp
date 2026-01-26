#include "UI.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "Alarm.h" // Assure-toi que l'include est bien là
#include <iostream>

UI::UI(SDL_Window* win, SDL_Renderer* rend)
    : window(win), renderer(rend)
{
    Initialize();
}

UI::~UI()
{
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
    
    ApplyCustomStyle();

    // Note: Si le fichier police est manquant, le programme peut crash ici
    io.Fonts->AddFontFromFileTTF("assets/fonts/Saira_Condensed-Bold.ttf", 18.0f);
        
    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) return false;
    if (!ImGui_ImplSDLRenderer3_Init(renderer)) return false;

    return true;
}

void UI::ApplyCustomStyle() 
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 12.0f;
    style.WindowBorderSize = 1.0f;
    style.FramePadding = ImVec2(8, 6);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]         = ImVec4(0.07f, 0.07f, 0.09f, 0.94f);
    colors[ImGuiCol_Header]           = ImVec4(0.20f, 0.25f, 0.35f, 1.00f); 
    colors[ImGuiCol_HeaderHovered]    = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_Button]           = ImVec4(0.20f, 0.25f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonHovered]    = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_FrameBg]          = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
    colors[ImGuiCol_CheckMark]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]       = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
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
                        bool& showDemo, Alarm& myAlarm)
{
    // Gestion du changement de thème
    static int lastTheme = -1;
    if (currentTheme != lastTheme) {
        UpdateTheme(currentTheme);
        lastTheme = currentTheme;
    }

    RenderMainMenuBar();

    ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::BeginTabBar("Tabs")) {
        if (ImGui::BeginTabItem("Horloge")) {
            RenderClockControls(use24hFormat, currentTheme, showAnalog, showDigital, analogScale, digitalScale, showDemo);
            ImGui::EndTabItem();
        }

        // Dans UI.cpp, dans l'onglet "Alarme"
if (ImGui::BeginTabItem("Alarme")) {
    ImGui::Text("Regler l'heure du reveil :");
    
    // Utilisation de InputInt au lieu de SliderInt pour une précision totale
    // On peut taper l'heure au clavier ou utiliser les petits boutons + / -
    ImGui::PushItemWidth(150); 
    if (ImGui::InputInt("Heures", &myAlarm.hour)) {
        if (myAlarm.hour > 23) myAlarm.hour = 0;
        if (myAlarm.hour < 0) myAlarm.hour = 23;
    }
    
    if (ImGui::InputInt("Minutes", &myAlarm.minute)) {
        if (myAlarm.minute > 59) myAlarm.minute = 0;
        if (myAlarm.minute < 0) myAlarm.minute = 59;
    }
    ImGui::PopItemWidth();

    ImGui::Separator();
    
    // Affichage clair de l'heure programmée
    ImGui::Text("Alarme fixee a : %02d:%02d", myAlarm.hour, myAlarm.minute);

    if (ImGui::Checkbox("Activer l'alarme", &myAlarm.active)) {
        if(!myAlarm.active) {
            myAlarm.snoozeActive = false;
            myAlarm.ringing = false;
        }
    }
    
    if (myAlarm.snoozeActive) {
        ImGui::TextColored(ImVec4(1,1,0,1), "Snooze actif pour %02d:%02d", myAlarm.snoozeHour, myAlarm.snoozeMinute);
    }
        ImGui::EndTabItem();
    }
        ImGui::EndTabBar();
    }
    ImGui::End();

    // --- POPUP DE SONNERIE ---
    if (myAlarm.ringing) {
        ImGui::OpenPopup("REVEIL !");
    }

    if (ImGui::BeginPopupModal("REVEIL !", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Il est %02d:%02d !", myAlarm.hour, myAlarm.minute);
        
        if (ImGui::Button("Arreter", ImVec2(120, 40))) {
            myAlarm.ringing = false;
            myAlarm.active = false;
            myAlarm.snoozeActive = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Snooze (10m)", ImVec2(120, 40))) {
            myAlarm.snoozeMinute = myAlarm.minute + 10;
            myAlarm.snoozeHour = myAlarm.hour;
            if (myAlarm.snoozeMinute >= 60) {
                myAlarm.snoozeMinute -= 60;
                myAlarm.snoozeHour = (myAlarm.snoozeHour + 1) % 24;
            }
            myAlarm.ringing = false;
            myAlarm.snoozeActive = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

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

void UI::UpdateTheme(int themeID) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    switch (themeID) {
        case 1: // THEME : NUIT
            colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.95f);
            colors[ImGuiCol_Button]   = ImVec4(0.10f, 0.30f, 0.50f, 1.00f);
            colors[ImGuiCol_Text]     = ImVec4(0.00f, 0.90f, 1.00f, 1.00f);
            break;
        case 2: // THEME : RETRO
            colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.12f, 0.10f, 1.00f);
            colors[ImGuiCol_Button]   = ImVec4(0.80f, 0.40f, 0.00f, 1.00f);
            colors[ImGuiCol_Text]     = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
            break;
        case 3: // THEME : MINIMAL
            ImGui::StyleColorsLight();
            style.WindowBorderSize = 1.0f;
            break;
        default: 
            ApplyCustomStyle(); 
            break;
    }
}

void UI::RenderClockControls(bool& use24hFormat, int& currentTheme, 
                            bool& showAnalog, bool& showDigital,
                            float& analogScale, float& digitalScale,
                            bool& showDemo)
{
    // Note: On ne met plus de ImGui::Begin ici car c'est déjà appelé dans un TabItem
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
        ImGui::Combo("Selection Theme", &currentTheme, themes, IM_ARRAYSIZE(themes));
    }

    ImGui::Separator();
    ImGui::Checkbox("Debug Demo ImGui", &showDemo);
}