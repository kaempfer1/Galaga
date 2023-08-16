#include "ship.h"
#include "enemy.h"
#include <cmath>
#include <iostream>

Missile::Missile() {
  sprite = up_missile_sprite;
}
Missile::~Missile() {
//  Mix_FreeMusic(fire_sound);
}

bool Missile::Active() {
  return alive;
}

void Missile::Active(bool hit) {
  alive = hit;
}

void Missile::Hit(bool hit) {
    alive = hit;
}

void Missile::PlaySound() {
  if (fire_sound == NULL) {
    fire_sound = Mix_LoadWAV("../resources/Fighter_Shot1.wav");
    if (fire_sound == NULL) {
      std::cerr << "Mix_LoadMUS could not load file.\n";
      std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
  }
  if (Mix_PlayChannel(1, fire_sound, 0) < 0) {
    std::cerr << "Mix_PlayMusic could not play file.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

// Return the current position of the missile rectangle
SDL_Rect Missile::GetRect() const {
    SDL_Rect position;
    
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

    return position;
}

// Update the missile position
void Missile::Update() {
  if (Active()) {
    y -= speed;                         // move missile up one frame
    if (y < 56) Active(false);          // if missile hits top of window, it missed
  }
}

Ship::Ship() {
  sprite = ship_sprite;
}

void Ship::Update() {
  if (alive) {
    if (hit > 0) {
      if (Die()) {
        if (lives >= 0) x = 281;                  // If player still has lives, re-center the ship to middle of screen
        else alive = false;
        sprite = ship_sprite;
      }
    }
    else {
      switch (direction) {
        case Direction::kLeft:
          x -= speed;
          if (x < 0) x = 0;                       // Stop at left edge of screen
          break;

        case Direction::kRight:
          x += speed;
          if (x > 560) x = 560;                   // Stop at right edge of screen (plus width of sprite)
          break;

        case Direction::kStop:
          break;
      }
  
      for(int i = 0; i < MAX_MISSILES; i++) {
        if (fire) {
          if (!missiles[i].Active()) {            // if fire button pressed & unused missiles available
            missiles[i].x = x + 16;               // center the missile starting position over the ship
            missiles[i].y = 668;
            missiles[i].Active(true);
            missiles[i].PlaySound();
            fire = false;
          }
        }
        missiles[i].Update();
      }
    }
  }
  if (score > highscore) highscore = score;
}

void Ship::Hit() {
  if (hit == 0) {
    hit = 1;
    PlaySound();
  }
}

// When the ship is hit, this will process the 4 exploding sprite graphics and return true when c complete.
bool Ship::Die() {
    switch (hit) {
        case 4:
            sprite = explode_1_sprite;
            break;
        case 8:
            sprite = explode_2_sprite;
            break;
        case 12:
            sprite = explode_3_sprite;
            break;
        case 16:
            sprite = explode_4_sprite;
            break;
    }
    hit++;
    if (hit > 16) {
        hit = 0;
        lives--;
        return true;
    } else return false;
}

void Ship::PlaySound() {
  if (die_sound == NULL) {
    die_sound = Mix_LoadMUS("../resources/Miss.wav");
    if (die_sound == NULL) {
      std::cerr << "Mix_LoadMUS could not load file.\n";
      std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
  }
  if (Mix_PlayMusic(die_sound, 0) < 0) {
    std::cerr << "Mix_PlayMusic could not play file.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

// Return the current position of the ship rectangle
SDL_Rect Ship::GetRect() const {
    SDL_Rect position;
    
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

    return position;
}
