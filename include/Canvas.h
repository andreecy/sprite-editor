#pragma once

#include <SDL2/SDL.h>
#include <EventManager.h>
#include <Vector2.h>
#include <Text.h>
#include <Pallete.h>

class Canvas : public EventReceiver {
public:
  Canvas(int x, int y, int width, int height);
  void updateTexture();
  void render();

  void handleEvent(SDL_Event &event) override {
    if (event.type == SDL_MOUSEMOTION)
    {
      globalMousePosition.x = event.motion.x;
      globalMousePosition.y = event.motion.y;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
      handleDraw(event);
    }
  }

  // get mouse position relative to canvas and scale of sprite
  Vector2 getMousePosition() {
    Vector2 posInCanvas = globalMousePosition - Vector2(x, y);
    return Vector2(posInCanvas.x / spriteScale, posInCanvas.y / spriteScale);
  }

  void setPallete(Pallete *pallete) {
    this->pallete = pallete;
  }

private:
  void handleDraw(SDL_Event &event);

private:
  SDL_Renderer *renderer;
  Pallete *pallete;
  // canvas position and size
  int x;
  int y;
  int width;
  int height;
  // sprite size and data
  int spriteScale = 8; // 1px = 8px
  int spriteWidth;
  int spriteHeight;
  Uint32 pixels;
  SDL_Texture *texture;
  Vector2 globalMousePosition = {0, 0};
};

Canvas::Canvas(int x, int y, int width, int height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;

  spriteWidth = 16;
  spriteHeight = 16;
  Uint32 pixels[spriteWidth * spriteHeight];

  for (int i = 0; i < spriteWidth * spriteHeight; i++) {
    pixels[i] = 0xFF000000; // ARGB
  }

  renderer = Renderer::getInstance().getRenderer();

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, spriteWidth, spriteHeight);
  if (texture == nullptr) {
      std::cerr << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
  }

  void *texturePixels;
  int pitch;
  if (SDL_LockTexture(texture, nullptr, &texturePixels, &pitch) != 0) {
    std::cerr << "Could not lock texture: " << SDL_GetError() << std::endl;
    SDL_DestroyTexture(texture);
  }
  memcpy(texturePixels, pixels, spriteWidth * spriteHeight * sizeof(Uint32));
  SDL_UnlockTexture(texture);
}

void Canvas::render() {
  SDL_Rect rect = {x, y, width, height};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
  SDL_RenderFillRect(renderer, &rect);

  // Draw texture
  SDL_Rect destRect = { x, y, spriteWidth, spriteHeight };
  SDL_RenderCopy(renderer, texture, nullptr, &destRect);

  SDL_Color color = {255, 255, 255, 255};
  char text[50];
  Vector2 mousePosition = getMousePosition();
  std::snprintf(text, sizeof(text), "%d, %d", mousePosition.x, mousePosition.y);
  renderText(text, 8, height + 16, color);
}

void Canvas::handleDraw(SDL_Event &event) {
  Vector2 targetPos = getMousePosition();
  // check if target position is within canvas
  if (targetPos.x >= 0 && targetPos.x < spriteWidth && targetPos.y >= 0 && targetPos.y < spriteHeight) {
    // get current color in pallete
    int colorIdx = pallete->getSelectedIndex();

    std::cout << "draw target: " << targetPos.x << ", " << targetPos.y << std::endl;
    std::cout << "with color index: " << colorIdx << std::endl;
  }
}
