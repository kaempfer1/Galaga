#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>

class Missile {
  public:
    Missile();
    ~Missile();

    bool Active();
    void Hit(bool hit);
    void PlaySound();
    void Active(bool hit);
    SDL_Rect GetRect() const;                               // Return the rectangle position of the sprite
    void Update();

    SDL_Rect sprite;
    float speed{12.0f};                                     // Missile update speed
    bool alive{false};
    int x;
    int y;
    int w = kSpriteScale / 7;                               // Missile sprite is very narrow
    int h = kSpriteScale;

  private:
    const static std::size_t kSpriteSize{16};               // Galage sprite size in pixels
    const static std::size_t kSpriteScale{42};              // Scale factor for sprites to game screen
    SDL_Rect up_missile_sprite{313, 118, 3, kSpriteSize };  // Up missile

    Mix_Chunk *fire_sound;
};

class Ship {
  public:
    enum class Direction { kLeft, kRight, kStop };

  Ship();
  ~Ship() {}

  void Update();
  void Start();
  void StageStart();
  void Hit();
  bool Die();
  void PlaySound();
  SDL_Rect GetRect() const;                                 // Return the rectangle position of the sprite

  Direction direction = Direction::kStop;

  SDL_Rect sprite;
  float speed{4.0f};
  int size{1};                                    // # of active player ships (1 or 2)
  int lives{2};                                   // Total # of player lives
  bool alive{true};
  bool fire{false};
  bool attack{false};                             // DEBUG 
  int x = 281;                                    // Center player ship on screen
  int y = 708;
  int w = kSpriteScale;
  int h = kSpriteScale;
  int score{0};
  int highscore{20000};
  int stage{0};
  bool stage_start = false;
  Mix_Music *die_sound;

  static const int MAX_MISSILES = 2;
  Missile missiles[MAX_MISSILES];

 private:
  int hit{0};                                               // Counter for the die/exploding process

  const static std::size_t kSpriteSize{16};                 // Galaga sprite size in pixels
  const static std::size_t kSpriteScale{42};                // Scale factor for sprites to game screen

  SDL_Rect ship_sprite{109, 1, kSpriteSize, kSpriteSize};           // Active player ship
  SDL_Rect explode_1_sprite{145, 1, kSpriteSize*2, kSpriteSize*2};  // Ship die sprite 1
  SDL_Rect explode_2_sprite{179, 1, kSpriteSize*2, kSpriteSize*2};  // Ship die sprite 2
  SDL_Rect explode_3_sprite{213, 1, kSpriteSize*2, kSpriteSize*2};  // Ship die sprite 3
  SDL_Rect explode_4_sprite{247, 1, kSpriteSize*2, kSpriteSize*2};  // Ship die sprite 4
};

#endif