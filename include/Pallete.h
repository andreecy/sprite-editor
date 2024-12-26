#pragma once

#include <SDL2/SDL.h>
#include <EventManager.h>

// https://lospec.com/palette-list/sweetie-16
const SDL_Color palette[16] = {
    {0x1a, 0x1c, 0x2c, 0xff}, {0x5d, 0x27, 0x5d, 0xff},
    {0xb1, 0x3e, 0x53, 0xff}, {0xef, 0x7d, 0x57, 0xff},
    {0xff, 0xcd, 0x75, 0xff}, {0xa7, 0xf0, 0x70, 0xff},
    {0x38, 0xb7, 0x64, 0xff}, {0x25, 0x71, 0x79, 0xff},
    {0x29, 0x36, 0x6f, 0xff}, {0x3b, 0x5d, 0xc9, 0xff},
    {0x41, 0xa6, 0xf6, 0xff}, {0x73, 0xef, 0xf7, 0xff},
    {0xf4, 0xf4, 0xf4, 0xff}, {0x94, 0xb0, 0xc2, 0xff},
    {0x56, 0x6c, 0x86, 0xff}, {0x33, 0x3c, 0x57, 0xff},
};

class Pallete : public EventReceiver {
private:
  int selectedIndex = 0;
  std::map<int, SDL_Rect> rectMap;
  int offsetX = 0;
  int offsetY = 0;

public:
  Pallete();
  void render(int x, int y);
  int getSelectedIndex();
  void setSelectedIndex(int index);
  SDL_Color getSelectedColor();
  SDL_Color getColor(int index);
  void handleEvent(SDL_Event &event) override {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      for (auto it = rectMap.begin(); it != rectMap.end(); ++it) {
        int i = it->first;
        SDL_Rect value = it->second;
        SDL_Rect rect = {value.x + offsetX, value.y + offsetY, value.w, value.h};
        SDL_Point point = {event.button.x, event.button.y};
        if (SDL_PointInRect(&point, &rect)) {
          selectedIndex = i;
          std::cout << "Selected index: " << selectedIndex << std::endl;
        }
      }
    }
  }
};

int Pallete::getSelectedIndex() {
  return selectedIndex;
}

void Pallete::setSelectedIndex(int index) {
  selectedIndex = index;
}

SDL_Color Pallete::getSelectedColor() {
  return palette[selectedIndex];
}

SDL_Color Pallete::getColor(int index) {
  return palette[index];
}

Pallete::Pallete() {
  int size = 16;
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      int posX = row * size;
      int posY = col * size;
      SDL_Rect rect = {posX, posY, size, size};
      int i = col * 4 + row;
      rectMap[i] = rect;
    }
  }
}

void Pallete::render(int x, int y){
  SDL_Renderer *renderer = Renderer::getInstance().getRenderer();
  offsetX = x;
  offsetY = y;

  for (auto it = rectMap.begin(); it != rectMap.end(); ++it) {
    int i = it->first;
    SDL_Color color = palette[i];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect value = it->second;
    SDL_Rect rect = {value.x + x, value.y + y, value.w, value.h};
    SDL_RenderFillRect(renderer, &rect);

    if (i == selectedIndex) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderDrawRect(renderer, &rect);
    }
  }
}
