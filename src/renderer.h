#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "ship.h"
#include "enemy.h"
#include <SDL.h>
//#include "/Library/Frameworks/SDL2_ttf.framework/Versions/A/Headers/SDL_ttf.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height);
  ~Renderer();

  void Render(Ship player, Enemy enemies);
  void UpdateWindowTitle(int fps);

 private:
  const std::size_t screen_width;
  const std::size_t screen_height;
  
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Texture *galaga_texture;
  double angle;

  SDL_Texture *header_texture;
  TTF_Font *font;                                           // Font for all screen text
  SDL_Color kRed    = {0xFF, 0x00, 0x00, 0xFF};             // Define colors for text
  SDL_Color kWhite  = {0xFF, 0xFF, 0xFF, 0xFF};
  SDL_Color kBlack  = {0x00, 0x00, 0x00, 0xFF};
  SDL_Color kLtBlue = {0x00, 0xFF, 0xFF, 0xFF};
  SDL_Rect header_position{40, 4, 380, 30};                 // Screen position for Player & High Score text
  SDL_Rect score_position{60, 32, 300, 30};                 // Screen position for player and high scores
//  SDL_Rect high_score_position{280, 32, 120, 30};           // Screen position for player and high scores
  SDL_Rect stage_start_position{200, 400, 180, 30};         // Screen position for Game Over text
  SDL_Rect game_over_position{200, 400, 200, 30};           // Screen position for Game Over text

  const static std::size_t kSpriteSize{16};                 // Galage sprite size in pixels
  const static std::size_t kSpriteScale{42};                // Scale factor for sprites to game screen

  // Define sprite locations on the Galaga sprite sheet in SDL format: {x, y, w, h}
  SDL_Rect sprite;                                                  // Generic sprite variable
  SDL_Rect live_sprite{289, 172, kSpriteSize, kSpriteSize};         // Ship sprite for # of lives
  SDL_Rect live_position{6, 756, kSpriteScale, kSpriteScale};       // Screen position for player lives
  SDL_Rect stage_sprite{307, 172, 8, kSpriteSize };                 // Game stage shield
  SDL_Rect stage_position{576, 756, kSpriteScale/2, kSpriteScale};  // Screen position for game stage
};

class Texture {       // DEBUG
  public:
    Texture(std::string text, std::string filename, int font_size, SDL_Color color);
    ~Texture();

    void Render();

  private:

};

#endif