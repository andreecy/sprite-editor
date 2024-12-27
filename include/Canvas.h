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
    switch(event.type)
    {
      case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT){
          drawing = true;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if(event.button.button == SDL_BUTTON_LEFT){
          drawing = false;
        }
        break;
      case SDL_MOUSEMOTION:
        globalMousePosition.x = event.motion.x;
        globalMousePosition.y = event.motion.y;
        break;
    }

    if(drawing){
      handleDrawing();
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
  void handleDrawing();

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
  Uint32 *pixels;
  SDL_Texture *texture;
  Vector2 globalMousePosition = {0, 0};
  bool drawing = false;
};

Canvas::Canvas(int x, int y, int width, int height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;

  spriteWidth = 16;
  spriteHeight = 16;
  pixels = new Uint32[spriteWidth * spriteHeight];

  // fill pixels with transparent
  for(int i = 0; i < spriteWidth * spriteHeight; i++ ) {
    pixels[i] = 0x00000000;
  }

  renderer = Renderer::getInstance().getRenderer();

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, spriteWidth, spriteHeight);
  if (texture == nullptr) {
      std::cerr << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
  }
}

void Canvas::render() {
  // Background of canvas
  SDL_Rect rect = {x, y, width, height};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
  SDL_RenderFillRect(renderer, &rect);

  // Draw sprite
  SDL_UpdateTexture(texture, NULL, pixels, spriteWidth * sizeof(Uint32));
  SDL_Rect destRect = { x, y, width, height };
  SDL_RenderCopy(renderer, texture, nullptr, &destRect);

  // text
  SDL_Color color = {255, 255, 255, 255};
  char text[50];
  Vector2 mousePosition = getMousePosition();
  std::snprintf(text, sizeof(text), "%d, %d", mousePosition.x, mousePosition.y);
  renderText(text, 8, height + 16, color);
}

void Canvas::handleDrawing() {
  Vector2 targetPos = getMousePosition();
  // check if target position is within canvas
  if (targetPos.x >= 0 && targetPos.x < spriteWidth && targetPos.y >= 0 && targetPos.y < spriteHeight) {
    // get current color in pallete
    int colorIdx = pallete->getSelectedIndex();
    Uint32 color = pallete->getColor(colorIdx);

    // std::cout << "draw target: " << targetPos.x << ", " << targetPos.y << std::endl;
    // std::cout << "with color index: " << colorIdx  << ", color: " << color << std::endl;
    pixels[targetPos.x + targetPos.y * spriteWidth] = color;
  }
}
