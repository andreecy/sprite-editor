#pragma once

#include <SDL2/SDL.h>

Uint32 ColorToUint32(const SDL_Color* color) {
    return (color->r << 24) | (color->g << 16) | (color->b << 8) | color->a;
}

SDL_Color Uint32ToColor(Uint32 color) {
    SDL_Color sdl_color;
    sdl_color.r = (color >> 24) & 0xFF;
    sdl_color.g = (color >> 16) & 0xFF;
    sdl_color.b = (color >> 8) & 0xFF;
    sdl_color.a = color & 0xFF;
    return sdl_color;
}
