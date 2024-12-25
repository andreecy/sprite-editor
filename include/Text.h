#pragma once

#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <Renderer.h>

// Structure to hold font character data
struct CharData {
  SDL_Rect rect;          // Rectangle within the font image
  int x_offset, y_offset; // Offset for proper character placement
  int x_advance;          // Distance to move for the next character
};

class Font {
private:
  SDL_Renderer *renderer;
  SDL_Texture *fontTexture;
  std::map<char, CharData> charMap;

public:
  Font();
  ~Font();

  bool loadFont(const std::string &filename);
  void renderText(const std::string &text, int x, int y, SDL_Color color);
};

Font::Font() {
  this->renderer = Renderer::getInstance().getRenderer();
  this->fontTexture = nullptr;
  this->charMap.clear();

  if (!loadFont("../fonts/white.png")) {
    std::cerr << "Failed to load font" << std::endl;
  }
}

Font::~Font() {
  if (fontTexture != nullptr) {
    SDL_DestroyTexture(fontTexture);
  }
}

// Function to load the bitmap font image
bool Font::loadFont(const std::string &filename) {
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if (surface == nullptr) {
    std::cerr << "Failed to load font image: " << IMG_GetError() << std::endl;
    return false;
  }

  // Convert surface to desired color format (e.g., SDL_PIXELFORMAT_RGBA8888)
  SDL_Surface *convertedSurface =
      SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
  SDL_FreeSurface(surface);
  if (convertedSurface == nullptr) {
    std::cerr << "Failed to convert surface: " << SDL_GetError() << std::endl;
    return false;
  }

  // Set alpha of black pixels to 0 (transparent)
  Uint32 blackColor = SDL_MapRGB(convertedSurface->format, 0, 0, 0);
  SDL_SetColorKey(convertedSurface, SDL_TRUE, blackColor);

  fontTexture = SDL_CreateTextureFromSurface(this->renderer, convertedSurface);
  SDL_FreeSurface(convertedSurface);
  if (fontTexture == nullptr) {
    std::cerr << "Failed to create font texture: " << SDL_GetError()
              << std::endl;
    return false;
  }

  // Assuming your font image has a specific layout (e.g., characters arranged
  // in rows and columns) You'll need to adjust this based on your font image's
  // structure
  int charWidth = 7;    // Example character width
  int charHeight = 9;   // Example character height
  int charsPerRow = 18; // Example characters per row

  int row = 0;
  int col = 0;
  for (char c = ' '; c <= '~'; ++c) { // Example character range
    CharData charData;
    charData.rect.x = col * charWidth;
    charData.rect.y = row * charHeight;
    // if(col > 0) charData.rect.x += margin;
    // if(row > 0) charData.rect.y += margin;
    charData.rect.w = charWidth;
    charData.rect.h = charHeight;
    charData.x_offset = 0;          // Adjust as needed
    charData.y_offset = 0;          // Adjust as needed
    charData.x_advance = charWidth; // Adjust as needed

    this->charMap[c] = charData;

    col++;
    if (col >= charsPerRow) {
      col = 0;
      row++;
    }
  }

  return true;
}

// Function to render text using the loaded font
void Font::renderText(const std::string &text, int x, int y, SDL_Color color) {
  SDL_SetTextureColorMod(fontTexture, color.r, color.g,
                         color.b); // Set color modulation
  int currentX = x;
  for (char c : text) {
    if (this->charMap.find(c) != this->charMap.end()) {
      const CharData &charData = this->charMap.at(c);

      SDL_Rect destRect = {currentX + charData.x_offset, y + charData.y_offset,
                           charData.rect.w, charData.rect.h};
      SDL_RenderCopy(this->renderer, fontTexture, &charData.rect, &destRect);

      currentX += charData.x_advance;
    }
  }
}
