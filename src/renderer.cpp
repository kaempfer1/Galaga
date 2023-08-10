#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height)
    : screen_width(screen_width),
      screen_height(screen_height) {

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize SDL_ttf for True Type Font rendering
  if (TTF_Init() < 0) {
    std::cerr << "SDL_ttf could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize SDL_image for PNG picture rendering
  if (IMG_Init(IMG_INIT_PNG) < 0) {
    std::cerr << "SDL_image could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize SDL_audio for sound rendering
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    std::cerr << "SDL_mixer could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Galaga", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);
  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  font = TTF_OpenFont("../resources/Emulogic.ttf", 24);                      // Get font for text
  if (font == NULL) {                                           // Check for errors
    std::cerr << "Font could not be created.\n";
    std::cerr << "SDL_Error: " << TTF_GetError() << "\n";
  }

  surface = TTF_RenderText_Solid(font, "1UP    HIGH SCORE", red);
  if (surface == NULL) {                                        // Check for errors
    std::cerr << "Text render to surface could not be created.\n";
    std::cerr << "SDL_Error: " << TTF_GetError() << "\n";
  }
  else {
    score_texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  }

  // Create texture for Galaga sprites
  surface = IMG_Load("../resources/GalagaSprites.png");
  if (surface == NULL) {                                            // Check for errors
    std::cerr << "GalagaSprites could not be created.\n";
    std::cerr << "SDL_Error: " << IMG_GetError() << "\n";
  }
  else {
    galaga_texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);  // Get texture of the Galage sprite sheet
    SDL_FreeSurface(surface);
  }

  // Create audio mixer
//  if (Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 2048) < 0) {
//    std::cerr << "SDL_mixer OpenAudio could not initialize.\n";
//    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
//  }
}

// Close everything
Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyTexture(texture);
  SDL_DestroyTexture(score_texture);
  SDL_DestroyTexture(galaga_texture);  
//  Mix_FreeMusic(shot);
//  Mix_CloseAudio();
  TTF_CloseFont(font);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

//void Renderer::Render(Ship const player, int const score) {
//void Renderer::Render(Ship const player) {
void Renderer::Render(Ship player, Enemy enemies) {

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(sdl_renderer);

  SDL_Rect rect;                                                // Destination for rectangle on screen

  // Render upper scoreboard information
  rect.x = 40;
  rect.y = 4;
  rect.w = 380;
  rect.h = 30;
  SDL_RenderCopy(sdl_renderer, score_texture, NULL, &rect);

  std::string str = std::to_string(player.score) + "     " + std::to_string(player.highscore);
  surface = TTF_RenderText_Solid(font, str.c_str(), white);
  texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  rect.x = 90;
  rect.y = 32;
  rect.w = 280;
  SDL_FreeSurface(surface);
  SDL_RenderCopy(sdl_renderer, texture, NULL, &rect);

  // Render Galaga sprites
  rect.w = kSpriteScale;                                                // Standard sprite width & height
  rect.h = kSpriteScale;

  // Render the active player ship & the player lives ships
  if (player.alive) {
    rect = player.GetRect();
    sprite = player.sprite;
    SDL_RenderCopy(sdl_renderer, galaga_texture, &sprite, &rect);         // Render texture

    rect.x = 6;                                                         // Location of # of lives
    rect.y = 756;
    for (int i = 0 ; i < (player.lives); i++) {
      SDL_RenderCopy(sdl_renderer, galaga_texture, &lives_sprite, &rect); // Render texture
      rect.x += kSpriteScale;
    }      
  }

  // Render the shields for the level of the game
  rect.x = 576;
  rect.y = 756;
  rect.w = kSpriteScale / 2;
  for (int i = 0 ; i < (player.level); i++) {
    SDL_RenderCopy(sdl_renderer, galaga_texture, &level_sprite, &rect);   // Render texture
    rect.x -= 23;
  }

  // *** Render enemy bugs and missiles ***
  for (int i = 0 ; i < Enemy::HIVE_SIZE; i++) {
    if (enemies.bugs[i].Active()) {
      rect = enemies.bugs[i].GetRect();
      sprite = enemies.bugs[i].sprite;
      angle = enemies.bugs[i].angle;
      SDL_RenderCopyEx(sdl_renderer, galaga_texture, &sprite, &rect, angle, NULL, SDL_FLIP_NONE);       // Render texture
    }
    if (enemies.bugs[i].missile.Active()) {
      rect = enemies.bugs[i].missile.GetRect();
      sprite = enemies.bugs[i].sprite;
      SDL_RenderCopy(sdl_renderer, galaga_texture, &sprite, &rect);
    }
  }

  // Render player ship missiles
  for(int i = 0; i < Ship::MAX_MISSILES; i++) {
    if (player.missiles[i].Active()) {
      rect = player.missiles[i].GetRect();
      sprite = player.missiles[i].sprite;
      SDL_RenderCopy(sdl_renderer, galaga_texture, &sprite, &rect);
    }
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int fps) {
  std::string title{"Frames per Second (FPS): " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

Texture::Texture(std::string text, std::string filename, int font_size, SDL_Color color) {}

Texture::~Texture() {}

void Texture::Render() {}
