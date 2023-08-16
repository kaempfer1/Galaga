#include "game.h"
#include <iostream>

void Collision::Update(Ship &player, Enemy &enemies) {
  SDL_Rect ship;
  SDL_Rect missile;
  SDL_Rect enemy;

  ship = player.GetRect();
  for (int j = 0 ; j < Enemy::HIVE_SIZE; j++) {       // Check for enemy missile collision with ship
    if (enemies.bugs[j].missile.Active()) {
      missile = enemies.bugs[j].missile.GetRect();
      if (SDL_HasIntersection(&missile, &ship)) {
        player.Hit();
        enemies.bugs[j].missile.Active(false);
      }
    }
    if (enemies.bugs[j].alive) {                      // Check for enemy bug collision with ship
      enemy = enemies.bugs[j].GetRect();
      if (SDL_HasIntersection(&enemy, &ship)) {
        player.Hit();
        enemies.bugs[j].hit++;
        if (enemies.bugs[j].hit == 1)
          player.score += enemies.bugs[j].Score();
      }
    }
  }
  for(int i = 0; i < Ship::MAX_MISSILES; i++) {       // Check for ship missile collision with bug
    if (player.missiles[i].Active()) {
      missile = player.missiles[i].GetRect();
      for (int j = 0 ; j < Enemy::HIVE_SIZE; j++) {
        if (enemies.bugs[j].alive) {
          enemy = enemies.bugs[j].GetRect();
          if (SDL_HasIntersection(&enemy, &missile)) {
            player.missiles[i].Active(false);               // Set missile inactive
            if (!enemies.bugs[j].shield) {                  // If bug doesn't have a shield, it's dead
              if (enemies.bugs[j].attack)                   // If bug is flying, it's worth double score
                player.score += 2 * enemies.bugs[j].Score();
              else player.score += enemies.bugs[j].Score();
              
            }
            enemies.bugs[j].Hit();
          }
        }
      }
    }
  } 
}

Game::Game()
    : player() {}

void Game::PlaySound() {
  if (stage_sound == NULL) {
    stage_sound = Mix_LoadMUS("../resources/Stage_Flag.wav");
    if (stage_sound == NULL) {
      std::cerr << "Mix_LoadWAV could not load file.\n";
      std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
  }
  if (Mix_PlayMusic(stage_sound, 0) < 0) {
    std::cerr << "Mix_PlayChannel could not play file.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

void Game::StartStage() {
    timer = 0;
    player.stage_start = false;
    enemies.Start();
    enemies.max_attackers = player.stage;
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  Uint32 level_start;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, player);
    Update();
    renderer.Render(player, enemies);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::Update() {
  if (!player.alive) return;

  player.Update();
  if (enemies.hive > 0) {
    enemies.Update();
    collision.Update(player, enemies);
  } else if (!timer) {
    player.stage_start = true;
    player.stage++;
    timer = SDL_GetTicks() + delay;
  } else if (SDL_GetTicks() > timer) StartStage();
}

int Game::GetScore() const { return player.score; }