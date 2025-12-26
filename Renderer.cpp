#include "Renderer.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Renderer::Renderer(SDL_Renderer* r) : renderer(r) {}

// Dessin du cadran analogique
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
        int inner = (i % 5 == 0) ? radius - 15 : radius - 8;

        int x1 = cx + cos(angle - M_PI / 2) * inner;
        int y1 = cy + sin(angle - M_PI / 2) * inner;
        int x2 = cx + cos(angle - M_PI / 2) * radius;
        int y2 = cy + sin(angle - M_PI / 2) * radius;

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

// Dessin des aiguilles
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

    // Minute (blanc)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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

// Dessin du cadre digital
void Renderer::DrawDigitalFrame(int x, int y, int w, int h, int hours, int minutes, int seconds)
{
    SDL_FRect rect = { (float)x, (float)y, (float)w, (float)h };

    // Fond
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Bordure
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderRect(renderer, &rect);

    // Pour l'instant, on n’affiche pas le texte (sera ajouté plus tard avec SDL_ttf)
}
