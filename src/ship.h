#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include "SDL.h"

class Missile {
  public:
    Missile() {}
    ~Missile() {}

    bool Active();
    void Active(bool hit);
    void Update();
    SDL_Rect GetRect() const;                       // Get the Rect of sprite
    void Hit(bool hit);

    float speed{12.0f};
    bool alive{false};
    int x;
    int y;
    int w = 6;
    int h = kSpriteScale;

  private:
    const static std::size_t kSpriteScale{42};      // Scale factor for sprites to game screen
};

class Ship {
  public:
    enum class Direction { kLeft, kRight, kStop };

  Ship() {}
  ~Ship() {}

  void Update();
  SDL_Rect GetRect() const;                       // Get the Rect of sprite

  Direction direction = Direction::kStop;

  float speed{4.0f};
  int size{1};                                    // Size of active shipe (1 or 2)
  int lives{2};                                   // Total # of player lives
  bool alive{true};
  bool fire{false};
  int x = 281;
  int y = 708;
  int w = kSpriteScale;
  int h = kSpriteScale;
  int score{0};
  int highscore{1000};
  int level{0};

  static const int MAX_MISSILES = 2;
  Missile missiles[MAX_MISSILES];

 private:
  const static std::size_t kSpriteScale{42};      // Scale factor for sprites to game screen
};

#endif