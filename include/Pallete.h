#pragma once

#include <SDL2/SDL.h>
#include <EventManager.h>
#include <Color.h>

// https://lospec.com/palette-list/sweetie-16
const Uint32 palette[16] = {
    0x1a1c2cff, 0x5d275dff,
    0xb13e53ff, 0xef7d57ff,
    0xffcd75ff, 0xa7f070ff,
    0x38b764ff, 0x257179ff,
    0x29366fff, 0x3b5dc9ff,
    0x41a6f6ff, 0x73eff7ff,
    0xf4f4f4ff, 0x94b0c2ff,
    0x566c86ff, 0x333c57ff,
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
  Uint32 getSelectedColor();
  Uint32 getColor(int index);
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

Uint32 Pallete::getSelectedColor() {
  return palette[selectedIndex];
}

Uint32 Pallete::getColor(int index) {
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
    SDL_Color color = Uint32ToColor(palette[i]);
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
