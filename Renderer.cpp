#include "Renderer.h"
#include <cmath>
#include <string>

#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void Renderer::initImGui(SDL_Window* window, SDL_Renderer* sdlRenderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForSDLRenderer(window, sdlRenderer);
    ImGui_ImplSDLRenderer3_Init(sdlRenderer);
}

void Renderer::shutdownImGui() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

Renderer::Renderer(SDL_Renderer* r) : renderer(r) {}

// === ANALOGIQUE===
void Renderer::DrawAnalogFrame(int cx, int cy, int radius)
{
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);

    for (int i = 0; i < 360; i++)
    {
        float rad1 = i * M_PI / 180.0f;
        float rad2 = (i + 1) * M_PI / 180.0f;

        int x1 = cx + cos(rad1) * radius;
        int y1 = cy + sin(rad1) * radius;
        int x2 = cx + cos(rad2) * radius;
        int y2 = cy + sin(rad2) * radius;

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }

    // Graduations
    for (int i = 0; i < 60; i++)
    {
        float angle = i * 6.0f * M_PI / 180.0f;

        int inner;

        if (i % 5 == 0) {
            inner = radius - 20; // heures
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // heures
            for (int dx = -2; dx <= 2; dx++)
            {
                for (int dy = -2; dy <= 2; dy++)
                {
                    SDL_RenderPoint(renderer, cx + dx, cy + dy);
                }
            }
        } else {
            inner = radius - 6;  // minutes
            SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255); // minutes
        }

        int x1 = cx + cos(angle - M_PI / 2) * inner;
        int y1 = cy + sin(angle - M_PI / 2) * inner;
        int x2 = cx + cos(angle - M_PI / 2) * radius;
        int y2 = cy + sin(angle - M_PI / 2) * radius;

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

void Renderer::DrawHands(int cx, int cy, int radius, int hours, int minutes, int seconds)
{
    float secAngle = (seconds * 6.0f - 90.0f) * M_PI / 180.0f;
    float minAngle = ((minutes + seconds / 60.0f) * 6.0f - 90.0f) * M_PI / 180.0f;
    float hourAngle = ((hours % 12 + minutes / 60.0f) * 30.0f - 90.0f) * M_PI / 180.0f;

    // Seconde (rouge)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderLine(renderer,
                       cx, cy,
                       cx + cos(secAngle) * radius * 0.9f,
                       cy + sin(secAngle) * radius * 0.9f);

    // Minute (gris)
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
    SDL_RenderLine(renderer,
                       cx, cy,
                       cx + cos(minAngle) * radius * 0.8f,
                       cy + sin(minAngle) * radius * 0.8f);

    // Heure (blanc)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer,
                       cx, cy,
                       cx + cos(hourAngle) * radius * 0.6f,
                       cy + sin(hourAngle) * radius * 0.6f);
}

// === DIGITALE AVEC CHIFFRES et CADRE RÉDUIT ===
void Renderer::DrawDigitalDisplay(int x, int y, int w, int h, int hours, int minutes, int seconds)
{
    // Fond du cadre digital - PLUS PETIT
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_FRect background = {static_cast<float>(x), 
                           static_cast<float>(y), 
                           static_cast<float>(w), 
                           static_cast<float>(h)};
    SDL_RenderFillRect(renderer, &background);
    
    // Bordure simple
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderRect(renderer, &background);
    
    // Afficher l'heure avec les chiffres
    int digitSize = 20; // Taille réduite pour cadre plus petit
    int startX = x + 10;
    int startY = y + (h - digitSize * 2) / 2;
    
    DrawDigitalTime(startX, startY, digitSize, hours, minutes, seconds);
}

// === TES FONCTIONS DE CHIFFRES (exactement comme tu les avais) ===
void Renderer::DrawDigit(int x, int y, int size, int digit)
{
    const bool segments[10][7] = {
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };

    SDL_SetRenderDrawColor(renderer, 0, 220, 0, 255); // Vert digital

    int segmentWidth = size / 5;
    int segmentLength = size * 3 / 2;
    
    const bool* seg = segments[digit];
    
    // Segment A (haut)
    if (seg[0]) {
        SDL_FRect rect = {static_cast<float>(x), 
                         static_cast<float>(y), 
                         static_cast<float>(segmentLength), 
                         static_cast<float>(segmentWidth)};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Segment B (haut droit)
    if (seg[1]) {
        SDL_FRect rect = {static_cast<float>(x + segmentLength - segmentWidth), 
                         static_cast<float>(y), 
                         static_cast<float>(segmentWidth), 
                         static_cast<float>(size)};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Segment C (bas droit)
    if (seg[2]) {
        SDL_FRect rect = {static_cast<float>(x + segmentLength - segmentWidth), 
                         static_cast<float>(y + size), 
                         static_cast<float>(segmentWidth), 
                         static_cast<float>(size)};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Segment D (bas)
    if (seg[3]) {
        SDL_FRect rect = {static_cast<float>(x), 
                         static_cast<float>(y + 2*size), 
                         static_cast<float>(segmentLength), 
                         static_cast<float>(segmentWidth)};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Segment E (bas gauche)
    if (seg[4]) {
        SDL_FRect rect = {static_cast<float>(x), 
                         static_cast<float>(y + size), 
                         static_cast<float>(segmentWidth), 
                         static_cast<float>(size)};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Segment F (haut gauche)
    if (seg[5]) {
        SDL_FRect rect = {static_cast<float>(x), 
                         static_cast<float>(y), 
                         static_cast<float>(segmentWidth), 
                         static_cast<float>(size)};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Segment G (milieu)
    if (seg[6]) {
        SDL_FRect rect = {static_cast<float>(x), 
                         static_cast<float>(y + size), 
                         static_cast<float>(segmentLength), 
                         static_cast<float>(segmentWidth)};
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Renderer::DrawColon(int x, int y, int size)
{
    // CHANGER ICI : Rouge au lieu de vert
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // ← ROUGE
    
    int dotSize = size / 2;
    int spacing = size / 1;
    
    // Point supérieur
    SDL_FRect topDot = {static_cast<float>(x), 
                       static_cast<float>(y), 
                       static_cast<float>(dotSize), 
                       static_cast<float>(dotSize)};
    
    // Point inférieur
    SDL_FRect bottomDot = {static_cast<float>(x), 
                          static_cast<float>(y + spacing), 
                          static_cast<float>(dotSize), 
                          static_cast<float>(dotSize)};
    
    SDL_RenderFillRect(renderer, &topDot);
    SDL_RenderFillRect(renderer, &bottomDot);
}

void Renderer::DrawDigitalTime(int x, int y, int size, int h, int m, int s)
{
    // Extraire chaque chiffre
    int h1 = h / 10;
    int h2 = h % 10;
    int m1 = m / 10;
    int m2 = m % 10;
    int s1 = s / 10;
    int s2 = s % 10;
    
    // Espacement entre les chiffres
    int spacing = size + 18;
    
    // Heures
    DrawDigit(x, y, size, h1);
    DrawDigit(x + spacing, y, size, h2);
    
    // Deux-points
    DrawColon(x + 2*spacing, y + size/2, size/2);
    
    // Minutes
    DrawDigit(x + 2*spacing + 15, y, size, m1);
    DrawDigit(x + 3*spacing + 15, y, size, m2);
    
    // Deux-points
    DrawColon(x + 4*spacing + 20, y + size/2, size/2);
    
    // Secondes
    DrawDigit(x + 4*spacing + 30, y, size, s1);
    DrawDigit(x + 5*spacing + 30, y, size, s2);
}