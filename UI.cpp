#include "UI.h"

UI::UI(SDL_Window* win, SDL_Renderer* rend)
    : window(win), renderer(rend)
{
    // Initialisation ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

UI::~UI()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void UI::BeginFrame()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void UI::Render()
{
    // Exemple simple : fenêtre flottante
    ImGui::Begin("Horloge Options");

    ImGui::Checkbox("Activer Alarme", &showAlarm);
    ImGui::Checkbox("Format 24h", &is24hFormat);

    ImGui::Text("Interface minimale prête !");
    ImGui::End();
}

void UI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}
