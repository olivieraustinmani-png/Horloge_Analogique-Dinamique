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
    
    // 1. Appliquer le style personnalisé
    ApplyCustomStyle();

    // 2. Charger la police (Assure-toi que le fichier existe à ce chemin)
    // Si le programme crash ici, commente la ligne suivante
    io.Fonts->AddFontFromFileTTF("assets/fonts/Saira_Condensed-Bold.ttf", 18.0f);
        
    // 3. Initialisation des ponts SDL3 et Renderer
    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        return false;
    }
    
    if (!ImGui_ImplSDLRenderer3_Init(renderer)) {
        return false;
    }

    return true;
}

void UI::ApplyCustomStyle() 
{
    ImGuiStyle& style = ImGui::GetStyle();
    
    // --- Formes et Arrondis ---
    style.WindowRounding = 8.0f;       // Fenêtres légèrement arrondies
    style.FrameRounding = 5.0f;        // Boutons et champs arrondis
    style.GrabRounding = 12.0f;        // Curseur des sliders circulaire
    style.WindowBorderSize = 1.0f;     // Bordure fine
    style.FramePadding = ImVec2(8, 6); // Plus d'espace dans les boutons

    // --- Palette de Couleurs (Deep Charcoal & Electric Blue) ---
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]         = ImVec4(0.07f, 0.07f, 0.09f, 0.94f); // Fond sombre
    colors[ImGuiCol_Header]           = ImVec4(0.20f, 0.25f, 0.35f, 1.00f); 
    colors[ImGuiCol_HeaderHovered]    = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_Button]           = ImVec4(0.20f, 0.25f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonHovered]    = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Bleu électrique
    colors[ImGuiCol_FrameBg]          = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
    colors[ImGuiCol_CheckMark]        = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
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
                        bool& showDemo)
{

    // On mémorise l'ancien thème pour savoir s'il a changé
    static int lastTheme = -1;
    if (currentTheme != lastTheme) {
        UpdateTheme(currentTheme);
        lastTheme = currentTheme;
    }

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
        case 1: // THEME : NUIT (Bleu sombre et Cyan)
            colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.95f);
            colors[ImGuiCol_Button]   = ImVec4(0.10f, 0.30f, 0.50f, 1.00f);
            colors[ImGuiCol_Text]     = ImVec4(0.00f, 0.90f, 1.00f, 1.00f);
            break;

        case 2: // THEME : RETRO (Orange et Gris)
            colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.12f, 0.10f, 1.00f);
            colors[ImGuiCol_Button]   = ImVec4(0.80f, 0.40f, 0.00f, 1.00f);
            colors[ImGuiCol_Text]     = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
            break;

        case 3: // THEME : MINIMAL (Blanc / Gris clair)
            ImGui::StyleColorsLight(); // Base claire
            style.WindowBorderSize = 1.0f;
            break;

        default: // THEME : CLASSIQUE (Celui que nous avons fait ensemble)
            ApplyCustomStyle(); 
            break;
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
        ImGui::Combo("Selection Theme", &currentTheme, themes, IM_ARRAYSIZE(themes));
    }

    ImGui::Separator();
    ImGui::Checkbox("Debug Demo ImGui", &showDemo);
    
    ImGui::End();
}

void UI::RenderThemePreview(int currentTheme)
{
    ImGui::Begin("Apercu Theme", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Theme actuel : %d", currentTheme);
    ImGui::End();
}