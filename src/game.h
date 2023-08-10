#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "ship.h"

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
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Ship player;
  Enemy enemies;
  Collision collision;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_bug;

  void Update();
};

#endif