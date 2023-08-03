#include "controller.h"
#include <iostream>
#include "SDL.h"

void Controller::HandleInput(bool &running, Ship &player) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_SPACE:                            // Space Bar is the Fire Button
          player.fire = true;
          break;

        case SDLK_LEFT:                             // Left arrow key moves left
        case SDLK_LCTRL:                            // Left CTRL key also moves left
          player.direction = Ship::Direction::kLeft;
          break;

        case SDLK_RIGHT:                           // Right arrow key moves right
        case SDLK_LALT:                            // Left ALT Key moves right
          player.direction = Ship::Direction::kRight;
          break;
      }
    }
    else if (e.type == SDL_KEYUP) {
      player.direction = Ship::Direction::kStop;
      player.fire = false;
    }
  }
}
