#include "enemy.h"

SDL_Rect Bug::GetRect() const {
    SDL_Rect position;
    
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;

    return position;
}

void Bug::Hit() {
    if (shield) shield = false;
    else hit = 1;
}

bool Bug::Active() {
  return alive;
}

void Bug::Attack() {

}

Enemy::Enemy() {};

void Enemy::Update() {
    // Position the BOSS bugs in platoon formation
    for (int i = 0 ; i < MAX_BOSS_BUGS; i++) {
        if (boss_bugs[i].alive) {
            boss_bugs[i].x = ((600 - ((MAX_BOSS_BUGS * 42) + (3 * kBugSpacing))) / 2) + ((42 + 4) * i);
            boss_bugs[i].y = kBugPlatoon_y;
        }
        if (boss_bugs[i].hit > 0) {
            boss_bugs[i].hit++;
            if (boss_bugs[i].hit > 4) {
                boss_bugs[i].hit = 0;
                boss_bugs[i].alive = false;
                hive--;
                }
            }
    }

    // Position the RED bugs
    for (int i = 0 ; i < MAX_RED_BUGS; i++) {
        if (red_bugs[i].alive) {
            if (i < (MAX_RED_BUGS / 2)) {
                red_bugs[i].x = ((600 - (((MAX_RED_BUGS / 2) * 42) + (7 * kBugSpacing))) / 2) + ((42 + 4) * i);
                red_bugs[i].y = kBugPlatoon_y + 42;
            } else {
                red_bugs[i].x = ((600 - (((MAX_RED_BUGS / 2) * 42) + (7 * kBugSpacing))) / 2) + ((42 + 4) * (i - (MAX_RED_BUGS / 2)));
                red_bugs[i].y = kBugPlatoon_y + (2 * 42);
            }
            if (red_bugs[i].hit > 0) {
                red_bugs[i].hit++;
                if (red_bugs[i].hit > 4) {
                    red_bugs[i].hit = 0;
                    red_bugs[i].alive = false;
                    hive--;
                }
            }
        }
    }

    // Update YELLOW bugs
    for (int i = 0 ; i < MAX_YELLOW_BUGS; i++) {
        if (yellow_bugs[i].alive) {
            if (yellow_bugs[i].hit > 0) {
                yellow_bugs[i].hit++;
                if (yellow_bugs[i].hit > 4) {
                    yellow_bugs[i].hit = 0;
                    yellow_bugs[i].alive = false;
                    hive--;
                }
            } else
                if (!yellow_bugs[i].attack) {
                    yellow_bugs[i].x = yellow_bugs[i].platoon_x;
                    yellow_bugs[i].y = yellow_bugs[i].platoon_y;
                } else {
            }
        }
    }
}

void Enemy::Start() {
    hive = 40;

    for (int i = 0 ; i < MAX_BOSS_BUGS; i++) {
        boss_bugs[i].alive = true;
        boss_bugs[i].shield = true;
    }

    for (int i = 0 ; i < MAX_RED_BUGS; i++) {
        red_bugs[i].alive = true;
        red_bugs[i].shield = false;
    }
    for (int i = 0 ; i < MAX_YELLOW_BUGS; i++) {
        yellow_bugs[i].alive = true;
        yellow_bugs[i].shield = false;
        if (i < (MAX_YELLOW_BUGS / 2)) {
            yellow_bugs[i].platoon_x = ((600 - (((MAX_YELLOW_BUGS / 2) * 42) + (8 * kBugSpacing))) / 2) + ((42 + 4) * i);
            yellow_bugs[i].platoon_y = kBugPlatoon_y + (3 * 42);
        } else {
            yellow_bugs[i].platoon_x = ((600 - (((MAX_YELLOW_BUGS / 2) * 42) + (8 * kBugSpacing))) / 2) + ((42 + 4) * (i - (MAX_YELLOW_BUGS / 2)));
            yellow_bugs[i].platoon_y = kBugPlatoon_y + (4 * 42);
        }
    }
}
