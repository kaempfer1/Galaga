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
            player.missiles[i].Active(false);
            if (!enemies.bugs[j].shield) {
              player.score += enemies.bugs[j].Score();
            }
            enemies.bugs[j].Hit();
          }
        }
      }
    }
  } 
}


Game::Game()
    : player(),
      engine(dev()) {}

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
  std::mt19937 engine(dev());
  std::uniform_int_distribution<int> random_bug(0, 39);
  
  if (!player.alive) return;

  player.Update();
  if (enemies.hive > 0) {
    enemies.Update();
    collision.Update(player, enemies);
    if (player.score > player.highscore) player.highscore = player.score;
    if (player.attack) {                  // DEBUG
      int x = random_bug(engine);
      enemies.bugs[x].attack = true;
      player.attack = false;
    }
  } else {
    enemies.Start();
    player.level++;
  }
}

int Game::GetScore() const { return player.score; }
int Game::GetSize() const { return player.size; }