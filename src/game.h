#ifndef GAME_H
#define GAME_H

#include <random>
#include <SDL.h>
#include "controller.h"
#include "renderer.h"
#include "ship.h"
#include "enemy.h"

class Collision {
  public:
    Collision() {}
    ~Collision() {}

    void Update(Ship &player, Enemy &enemies);

  private:
};

class Game {
 public:
  Game();
  ~Game() {}

  void StartStage();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  void PlaySound();
  void Update();

  Ship player;
  Enemy enemies;
  Collision collision;

  Uint32 timer{0};
  Uint32 delay = 1000;

  Mix_Music *stage_sound;
};

#endif