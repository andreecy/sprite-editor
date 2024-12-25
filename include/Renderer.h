#pragma once

#include <SDL2/SDL.h>

class Renderer {
public:
  static Renderer &getInstance(){
    static Renderer instance;
    return instance;
  }

  void setRenderer(SDL_Renderer *renderer){
    _renderer = renderer;
  }

  SDL_Renderer *getRenderer(){
    return _renderer;
  }

private:
  Renderer(){}
  SDL_Renderer *_renderer = nullptr;
};
