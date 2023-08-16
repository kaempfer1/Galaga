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
  if (Mix_Init(MIX_INIT_FLAC) < 0) {
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

  // Create the screen text font
  font = TTF_OpenFont("../resources/Emulogic.ttf", 24);                 // Get font for text
  if (font == NULL) {                                                   // Check for errors
    std::cerr << "Font could not be created.\n";
    std::cerr << "SDL_Error: " << TTF_GetError() << "\n";
  }
  else {
    // Create the texture for the title header text
    surface = TTF_RenderText_Solid(font, "1UP    HIGH SCORE", kRed);
    if (surface == NULL) {                                              // Check for errors
      std::cerr << "Text render to surface could not be created.\n";
      std::cerr << "SDL_Error: " << TTF_GetError() << "\n";
    }
    else {
      header_texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
      SDL_FreeSurface(surface);
    }
  }

  // Create texture for Galaga sprite sheet
  surface = IMG_Load("../resources/GalagaSprites.png");  
  if (surface == NULL) {                                                // Check for errors
    std::cerr << "GalagaSprites could not be created.\n";
    std::cerr << "SDL_Error: " << IMG_GetError() << "\n";
  }
  else {
    Uint32 color_key = SDL_MapRGB(surface->format, 0x00, 0x00, 0x00);     // Remove the sprite sheet black background
    SDL_SetColorKey(surface, SDL_TRUE, color_key);                        // Idea from stackoverflow
    galaga_texture = SDL_CreateTextureFromSurface(sdl_renderer, surface); // Get texture of the Galage sprite sheet
    SDL_FreeSurface(surface);
  }

  // Create audio mixer
//  if (Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 2048) < 0) {
  if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "Mix_OpenAudio could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

// Close everything
Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyTexture(texture);
  SDL_DestroyTexture(header_texture);
  SDL_DestroyTexture(galaga_texture);  
//  Mix_FreeMusic(ship_shot);
  Mix_CloseAudio();
  TTF_CloseFont(font);
  TTF_Quit();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Renderer::Render(Ship player, Enemy enemies) {

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render upper scoreboard title information
  SDL_RenderCopy(sdl_renderer, header_texture, NULL, &header_position);

  // Render the player's score and high score
  std::string str = std::to_string(player.score) + "     " + std::to_string(player.highscore);
  surface = TTF_RenderText_Solid(font, str.c_str(), kWhite);
  texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  SDL_RenderCopy(sdl_renderer, texture, NULL, &score_position);           // Render texture
//  str = std::to_string(player.highscore);                                 // High score
//  surface = TTF_RenderText_Solid(font, str.c_str(), kWhite);
 // texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  SDL_FreeSurface(surface);
//  SDL_RenderCopy(sdl_renderer, texture, NULL, &high_score_position);      // Render texture

  // Render the shields for the stage of the game
  SDL_Rect rect = stage_position;                                         // SDL Rectangle for screen position
  for (int i = 0 ; i < (player.stage); i++) {
    SDL_RenderCopy(sdl_renderer, galaga_texture, &stage_sprite, &rect);   // Render texture
    rect.x -= kSpriteScale / 2;
  }

  // Set default Galaga sprite scale for the screen
  rect.w = kSpriteScale;                                                  // Standard sprite width & height
  rect.h = kSpriteScale;

  // Render the "STAGE #" text
  if (player.stage_start) {
    str = "Stage " + std::to_string(player.stage);
    surface = TTF_RenderText_Solid(font, str.c_str(), kLtBlue);
    texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(sdl_renderer, texture, NULL, &stage_start_position);           // Render texture
    SDL_DestroyTexture(texture);
  }

  // Render the active player ship & the player lives ships
  if (player.alive) {
    rect = player.GetRect();                                              // Get current player ship location
    SDL_RenderCopy(sdl_renderer, galaga_texture, &player.sprite, &rect);  // Render texture

    rect = live_position;
    for (int i = 0 ; i < (player.lives); i++) {
      SDL_RenderCopy(sdl_renderer, galaga_texture, &live_sprite, &rect);  // Render texture
      rect.x += kSpriteScale;
    }      
  } else {
    // Render the "GAME OVER" text
    surface = TTF_RenderText_Solid(font, "GAME OVER", kLtBlue);
    texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(sdl_renderer, texture, NULL, &game_over_position);           // Render texture
  }

  // *** Render the player's missiles ***
  for(int i = 0; i < Ship::MAX_MISSILES; i++) {
    if (player.missiles[i].Active()) {
      rect = player.missiles[i].GetRect();
      sprite = player.missiles[i].sprite;
      SDL_RenderCopy(sdl_renderer, galaga_texture, &sprite, &rect);     // Render texture
    }
  }

  // *** Render enemy bugs and their missiles ***
  for (int i = 0 ; i < Enemy::HIVE_SIZE; i++) {
    if (enemies.bugs[i].Active()) {                                       // If bug is active/alive
      rect = enemies.bugs[i].GetRect();
      sprite = enemies.bugs[i].sprite;
      angle = enemies.bugs[i].angle;                                      // Set rotation angle
      SDL_RenderCopyEx(sdl_renderer, galaga_texture, &sprite, &rect, angle, NULL, SDL_FLIP_NONE); // Render texture
    }
    if (enemies.bugs[i].missile.Active()) {                               // If bug's missile is in air
      rect = enemies.bugs[i].missile.GetRect();
      sprite = enemies.bugs[i].missile.sprite;
      SDL_RenderCopy(sdl_renderer, galaga_texture, &sprite, &rect);       // Render texture
    }
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int fps) {
  std::string title{"Frames per Second (FPS): " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}


// DEBUG
Texture::Texture(std::string text, std::string filename, int font_size, SDL_Color color) {}

Texture::~Texture() {}

void Texture::Render() {}
