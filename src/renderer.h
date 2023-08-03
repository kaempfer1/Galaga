#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "ship.h"
#include "enemy.h"
#include <SDL.h>
//#include "/Library/Frameworks/SDL2_ttf.framework/Versions/A/Headers/SDL_ttf.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
//#include <SDL_mixer.h>
#include "/Library/Frameworks/SDL2_mixer.framework/Versions/A/Headers/SDL_mixer.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height);
  ~Renderer();

//  void Render(Ship const player, Missile const missile, SDL_Point const &food, int const score);
//  void Render(Ship const player, SDL_Point const &food, int const score);
//  void Render(Ship const player, int const score);
//  void Render(Ship const player);
  void Render(Ship player, Enemy enemies);
  void UpdateWindowTitle(int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Texture *galaga_texture;
  SDL_Texture *score_texture;
  TTF_Font *font;
//  SDL_Color red = {0xC8, 0x00, 0x00, 0xFF};
  SDL_Color red = {0xFF, 0x00, 0x00, 0xFF};
  SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};

  const static std::size_t kSpriteSize{16};                 // Galage sprite size in pixels
  const static std::size_t kSpriteScale{42};                // Scale factor for sprites to game screen

  const std::size_t screen_width;
  const std::size_t screen_height;

  // Define sprite locations on the Galaga sprite sheet in SDL format: {x, y, w, h}
  SDL_Rect sprite;                                                  // Generic sprite
  SDL_Rect ship_sprite{109, 1, kSpriteSize, kSpriteSize};           // Active player ship
  SDL_Rect lives_sprite{289, 172, kSpriteSize, kSpriteSize};        // Ship for # of lives
  SDL_Rect level_sprite{307, 172, 8, kSpriteSize };                 // Game level shields
  SDL_Rect boss_sprite{109, 37, kSpriteSize, kSpriteSize};          // Boss enemy
  SDL_Rect boss_blue_sprite{109, 55, kSpriteSize, kSpriteSize};     // Boss blue enemy
  SDL_Rect red_sprite{109, 73, kSpriteSize, kSpriteSize};           // Red enemy
  SDL_Rect yellow_sprite{109, 91, kSpriteSize, kSpriteSize};        // Yellow enemy
  SDL_Rect up_missile_sprite{313, 118, 3, kSpriteSize };            // Up missile
  SDL_Rect down_missile_sprite{313, 136, 3, kSpriteSize };          // Down missile
  SDL_Rect explode_1_sprite{289, 1, kSpriteSize*2, kSpriteSize*2};  // Yellow enemy
  SDL_Rect explode_2_sprite{323, 1, kSpriteSize*2, kSpriteSize*2};  // Yellow enemy
  SDL_Rect explode_3_sprite{357, 1, kSpriteSize*2, kSpriteSize*2};  // Yellow enemy
  SDL_Rect explode_4_sprite{391, 1, kSpriteSize*2, kSpriteSize*2};  // Yellow enemy
};

class Texture {
  public:
    Texture(std::string text, std::string filename, int font_size, SDL_Color color);
    ~Texture();

    void Render();

  private:

};

#endif