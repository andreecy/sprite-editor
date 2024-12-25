#pragma once

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdio>

#include <Text.h>
#include <Canvas.h>
#include <Vector2.h>
#include <Renderer.h>
#include <Pallete.h>

class App
{
public:
  App();
  void update();

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  int width;
  int height;
  int windowWidth;
  int windowHeight;
  bool quit = false;

  SDL_Event event;
  Canvas *canvas;
  Font *font;
  Pallete *pallete;
};

App::App()
{
  width = 320;
  height = 240;
  windowWidth = width * 3;
  windowHeight = height * 3;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError()
              << std::endl;
  }

  // Create a window
  window = SDL_CreateWindow("Sprite Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            windowWidth, windowHeight, SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    std::cerr << "Window could not be created! SDL Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
  }

  // Create a renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
  {
    std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError()
              << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  Renderer::getInstance().setRenderer(renderer);

  SDL_RenderSetLogicalSize(renderer, width, height);

  canvas = new Canvas();
  font = new Font();
  pallete = new Pallete();
}

// Game loop
void App::update()
{

  Vector2 mousePos = {0, 0};

  while (!quit)
  {
    // Handle events
    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_QUIT)
      {
        quit = true;
      }
      else if (event.type == SDL_MOUSEMOTION)
      {
        mousePos.x = event.motion.x;
        mousePos.y = event.motion.y;
      }
    }

    pallete->update();

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    canvas->render(8, 8, 128, 128);
    pallete->render(128 + 16, 8);

    SDL_Color color = {255, 255, 255, 255};
    char text[50];
    std::snprintf(text, sizeof(text), "%d, %d", mousePos.x, mousePos.y);
    font->renderText(text, 8, height - 16, color);

    // Present the rendered frame
    SDL_RenderPresent(renderer);
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
