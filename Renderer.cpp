#include "Renderer.h"
#include <cmath>

Renderer::Renderer(SDL_Renderer* r)
    : renderer(r) {}

void Renderer::DrawAnalogFrame(int cx, int cy, int radius)
{
    // 🔵 Cercle
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

    // 🔸 Graduations
    for (int i = 0; i < 60; i++)
    {
        float angle = i * 6.0f * M_PI / 180.0f;
        int inner = (i % 5 == 0) ? radius - 15 : radius - 8;

        int x1 = cx + cos(angle - M_PI/2) * inner;
        int y1 = cy + sin(angle - M_PI/2) * inner;
        int x2 = cx + cos(angle - M_PI/2) * radius;
        int y2 = cy + sin(angle - M_PI/2) * radius;

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

void Renderer::DrawDigitalFrame(int x, int y, int w, int h)
{
    SDL_FRect rect = { (float) x, (float) y, (float) w, (float) h };

    // Fond
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Bordure
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderRect(renderer, &rect);
}
