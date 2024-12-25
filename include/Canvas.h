#pragma once

#include <SDL2/SDL.h>

class Canvas {
public:
  void render(int x, int y, int width, int height);
};

void Canvas::render(int x, int y, int width, int height) {
  SDL_Renderer *renderer = Renderer::getInstance().getRenderer();
  SDL_Rect rect = {x, y, width, height};
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderFillRect(renderer, &rect);
}
