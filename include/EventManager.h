#pragma once

#include <SDL2/SDL.h>
#include <vector>

class EventReceiver {
public:
  virtual void handleEvent(SDL_Event &event) = 0;
};

class EventManager {
private:
  std::vector<EventReceiver *> receivers;

public:
  void addReceiver(EventReceiver *receiver);
  void removeReceiver();
  void handleEvent(SDL_Event &event);
};

void EventManager::addReceiver(EventReceiver *receiver) {
  receivers.push_back(receiver);
}

void EventManager::removeReceiver() {
  receivers.pop_back();
}

void EventManager::handleEvent(SDL_Event &event) {
  for (EventReceiver *receiver : receivers) {
    receiver->handleEvent(event);
  }
}
