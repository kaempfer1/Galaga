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
  SDL_Color red = {0xFF, 0x00, 0x00, 0xFF};
  SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
  double angle;

  const static std::size_t kSpriteSize{16};                 // Galage sprite size in pixels
  const static std::size_t kSpriteScale{42};                // Scale factor for sprites to game screen

  const std::size_t screen_width;
  const std::size_t screen_height;
  
  // Define sprite locations on the Galaga sprite sheet in SDL format: {x, y, w, h}
  SDL_Rect sprite;                                                  // Generic sprite
  SDL_Rect lives_sprite{289, 172, kSpriteSize, kSpriteSize};        // Ship for # of lives
  SDL_Rect level_sprite{307, 172, 8, kSpriteSize };                 // Game level shields
};

class Texture {
  public:
    Texture(std::string text, std::string filename, int font_size, SDL_Color color);
    ~Texture();

    void Render();

  private:

};

#endif