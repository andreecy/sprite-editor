#pragma once

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <cstdio>

#include <Canvas.h>
#include <Renderer.h>
#include <Pallete.h>
#include <EventManager.h>

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

  EventManager *eventManager;
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

  pallete = new Pallete();
  canvas = new Canvas(8, 8, 128, 128);
  canvas->setPallete(pallete);

  eventManager = new EventManager();
  eventManager->addReceiver(pallete);
  eventManager->addReceiver(canvas);
}

// Game loop
void App::update()
{
  SDL_Event event;
  while (!quit)
  {
    // Handle events
    while (SDL_PollEvent(&event) != 0)
    {
      eventManager->handleEvent(event);

      // handle quit event
      if (event.type == SDL_QUIT)
      {
        quit = true;
        break;
      }
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    canvas->render();
    pallete->render(128 + 16, 8);

    // Present the rendered frame
    SDL_RenderPresent(renderer);
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
