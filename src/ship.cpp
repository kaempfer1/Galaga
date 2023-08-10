#include "ship.h"
#include "enemy.h"
#include <cmath>
#include <iostream>

Missile::Missile() {
  sprite = up_missile_sprite;
}

bool Missile::Active() {
  return alive;
}

void Missile::Active(bool hit) {
  alive = hit;
}

void Missile::Update() {
  if (Active()) {
    y -= speed;                         // move missile up one frame
    if (y < 56) Active(false);          // if missile hits top of window, it missed

//    if (y < 56) {
//      std::cerr << "Missile MISSED,    " << y << "\n";
//      Active(false);                  // if missile hits top of window, it missed
//    } else std::cerr << "Missile is ALIVE,      " << y << "\n";
  }
}

SDL_Rect Missile::GetRect() const {
    SDL_Rect position;
    
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

    return position;
}

void Missile::Hit(bool hit) {
    alive = hit;
//    std::cerr << "Missile is dead!,   " << y << "\n";
}

Ship::Ship() {
  sprite = ship_sprite;
}

void Ship::Update() {
  if (alive) {
    if (hit > 0) {
      if (Die()) {
        if (lives >= 0) {
          sprite = ship_sprite;
          x = 281;
        }
        else alive = false;
      }
    }
    else {
      switch (direction) {
        case Direction::kLeft:
          x -= speed;
          if (x < 0) x = 0;
          break;

        case Direction::kRight:
          x += speed;
          if (x > 560) x = 560;
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
            fire = false;
          }
        }
        missiles[i].Update();
      }
    }
  }
}

void Ship::Hit() {
    if (hit == 0) hit = 1;
}

bool Ship::Die() {
    switch (hit) {
        case 1:
            sprite = explode_1_sprite;
            break;
        case 2:
            sprite = explode_2_sprite;
            break;
        case 3:
            sprite = explode_3_sprite;
            break;
        case 4:
            sprite = explode_4_sprite;
            break;
    }
    hit++;
    if (hit > 4) {
        hit = 0;
        lives--;
        return true;
    } else return false;
}

SDL_Rect Ship::GetRect() const {
    SDL_Rect position;
    
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

    return position;
}
