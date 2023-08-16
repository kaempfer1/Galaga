#include "controller.h"
#include <iostream>
#include "SDL.h"

// ===========================================================================
// Method to handle keyboard inputs using the SDL_PollEvent library 
//
// Player ship movement is controlled by the left/right arrow keys or the
// left side CTRL and ALT keys (for left handed people).
// Ship missile firing is from the SPACE bar.
// ===========================================================================
void Controller::HandleInput(bool &running, Ship &player) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {             // Handle the key presses
      switch (e.key.keysym.sym) {
        case SDLK_SPACE:                            // Space Bar is the Fire Button
          player.fire = true;
          break;

        case SDLK_LEFT:                             // Left arrow key moves left
        case SDLK_LCTRL:                            // Left CTRL key also moves left
          player.direction = Ship::Direction::kLeft;
          break;

        case SDLK_RIGHT:                           // Right arrow key moves right
        case SDLK_LALT:                            // Left ALT Key also moves right
          player.direction = Ship::Direction::kRight;
          break;

        case SDLK_s:                      //  DEBUG
          player.lives = 2;
          player.alive = true;
          break;
      }
    }
    else if (e.type == SDL_KEYUP) {                 // Handle the key releases
      switch (e.key.keysym.sym) {
        case SDLK_SPACE:                            // Space Bar is the Fire Button
          player.fire = false;
          break;

        case SDLK_LEFT:                             // Left arrow key moves left
        case SDLK_LCTRL:                            // Left CTRL key also moves left
          player.direction = Ship::Direction::kStop;
          break;

        case SDLK_RIGHT:                           // Right arrow key moves right
        case SDLK_LALT:                            // Left ALT Key also moves right
          player.direction = Ship::Direction::kStop;
          break;
      }
    }
  }
}
