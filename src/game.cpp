#include "game.h"
#include <iostream>

void Collision::Update(Ship &player, Enemy &enemies) {
  for(int i = 0; i < Ship::MAX_MISSILES; i++) {
    if (player.missiles[i].Active()) {
      SDL_Rect missile;
      missile = player.missiles[i].GetRect();
      SDL_Rect enemy;
      for (int j = 0 ; j < Enemy::MAX_YELLOW_BUGS; j++) {
        if (enemies.yellow_bugs[j].alive) {
          enemy = enemies.yellow_bugs[j].GetRect();
          if (SDL_HasIntersection(&enemy, &missile)) {
            player.missiles[i].Active(false);
            enemies.yellow_bugs[j].Hit();
            player.score += Enemy::YELLOW_SCORE;
          }
        }
      }
      for (int j = 0 ; j < Enemy::MAX_RED_BUGS; j++) {
        if (enemies.red_bugs[j].alive) {
          enemy = enemies.red_bugs[j].GetRect();
          if (SDL_HasIntersection(&enemy, &missile)) {
            player.missiles[i].Active(false);
            enemies.red_bugs[j].Hit();
            player.score += Enemy::RED_SCORE;
          }
        }
      }
      for (int j = 0 ; j < Enemy::MAX_BOSS_BUGS; j++) {
        if (enemies.boss_bugs[j].alive) {
          enemy = enemies.boss_bugs[j].GetRect();
          if (SDL_HasIntersection(&enemy, &missile)) {
            player.missiles[i].Active(false);
            if (!enemies.boss_bugs[j].shield)
              player.score += Enemy::BOSS_SCORE;
            enemies.boss_bugs[j].Hit();
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

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, player);
    Update();
//    renderer.Render(player, missile, food, score);
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
    if (player.score > player.highscore) player.highscore = player.score;
  } else {
    enemies.Start();
    player.level++;
  }
}

int Game::GetScore() const { return player.score; }
int Game::GetSize() const { return player.size; }