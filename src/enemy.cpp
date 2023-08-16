#include "enemy.h"
#include <iostream>

Bug::Bug() {
}

SDL_Rect Bug::GetRect() const {
    SDL_Rect rectangle;
    
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

    return rectangle;
}

// Get the position of the bug
SDL_Point Bug::Pos() const {
    SDL_Point position;

    position.x = x;
    position.y = y;
    
    return position;
}

// Set the position of the bug
void Bug::Pos(SDL_Point p) {
    x = p.x;
    y = p.y;
}

bool Bug::Active() {
  return alive;
}

// Handle missile hit to bugs. BOSS bugs take 2 hits to die.
void Bug::Hit() {
    if (shield) {
        sprite = boss_blue_sprite;
        shield = false;

        if (shield_sound == NULL) {
            shield_sound = Mix_LoadWAV("../resources/Boss_Stricken#1.wav");
            if (shield_sound == NULL) {
                std::cerr << "Mix_LoadWAV could not load file.\n";
                std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
            }
        }
        if (Mix_PlayChannel(1, shield_sound, 0) < 0) {
            std::cerr << "Mix_PlayChannel could not play file.\n";
            std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        }
    } else hit = 1;
}

bool Bug::Die() {
    switch (hit) {
        case 2:
            sprite = explode_1_sprite;
            break;
        case 4:
            sprite = explode_2_sprite;
            break;
        case 6:
            sprite = explode_3_sprite;
            break;
        case 8:
            sprite = explode_4_sprite;
            break;
    }
    hit++;
    if (hit > 8) {
        hit = 0;
        alive = false;
        return true;
    } else return false;
}

void Bug::GetSound(const char *file) {
    if (die_sound == NULL) {
        die_sound = Mix_LoadWAV(file);
        if (die_sound == NULL) {
            std::cerr << "Mix_LoadWAV could not load file.\n";
            std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        }
    }
}

void Bug::PlaySound() {
    if (Mix_PlayChannel(1, die_sound, 0) < 0) {
        std::cerr << "Mix_PlayChannel could not play file.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
}

void Bug::Dive() {
    if (dive_pos == 10) {                   // Fire after bug launches from their dive
        missile.Active(true);
        missile.x = x + 21;                 // Fire from bottom of bug sprite
        missile.y = y + 21;
        }

    if (dive_pos < DIVE_SIZE) {
        if (dive_path == J_PATTERN) {
            if (dive_side == LEFT) {
                x -= pattern[dive_pos].x * speed;
                angle = -pattern[dive_pos].a;
            }
            else {
                x += pattern[dive_pos].x * speed;
                angle = pattern[dive_pos].a;
            }
            y += pattern[dive_pos].y * speed;
        } else if (dive_path == S_PATTERN) {
            if (dive_side == LEFT) {
                x -= pattern2[dive_pos].x * speed;
                angle = -pattern2[dive_pos].a;
            }
            else {
                x += pattern2[dive_pos].x * speed;
                angle = pattern2[dive_pos].a;
            }
            y += pattern2[dive_pos].y * speed;
            if (y > 800) y = 0;
        } else if (dive_path == O_PATTERN) {
            if (dive_side == LEFT) {
                x -= pattern3[dive_pos].x * speed;
                angle = -pattern3[dive_pos].a;
            }
            else {
                x += pattern3[dive_pos].x * speed;
                angle = pattern3[dive_pos].a;
            }
            y += pattern3[dive_pos].y * speed;
            if (y > 800) {
                x = home.x;
                y = 52;
            }
        }
        dive_pos++;
    } else {
        x = home.x;
        y = home.y;
        angle = 0;
        dive_pos = 0;
        attack = false;
        fire = false;
    }
}

int Bug::Score() {
    return value;
}

Enemy::Enemy() {
    std::random_device dev;
    std::mt19937 engine(dev());
};

// Update the bug positions & missiles
void Enemy::Update() {
    int direction;
    std::random_device dev;
    std::mt19937 engine(dev());
    std::uniform_int_distribution<int> random_bug(0, HIVE_SIZE-1);

    if (attackers < max_attackers) {
        int x = random_bug(engine);
        bugs[x].attack = true;
    } 
    attackers = 0;
    for (int i = 0 ; i < HIVE_SIZE; i++) {
        if (bugs[i].alive) {
            if (bugs[i].hit > 0) {                      // if bug is hit
                if (bugs[i].Die()) {                    // start the die / explode graphics
                    bugs[i].PlaySound();
                    hive--;
                }
            } else if (bugs[i].attack) {
                attackers++;
                bugs[i].Dive();
            }
        }
        if (bugs[i].missile.Active()) {                 // if bug already fired
            bugs[i].missile.y += bugs[i].missile.speed; // then update missile position
            if (bugs[i].missile.y > 800) 
                bugs[i].missile.Active(false);          // if missile off the screen, it missed
        }
    }
}

// Setup a new bug platoon
void Enemy::Start() {
    hive = HIVE_SIZE;
    int row = 0;                            // Counter for row # in formation
    attackers = 0;

    for (int i = 0 ; i < HIVE_SIZE; i++) {
        // BOSS bugs in top row
        bugs[i].alive = true;
        if (i < MAX_BOSS_BUGS) {
            bugs[i].shield = true;
            bugs[i].value = BOSS_SCORE;
            bugs[i].sprite = boss_sprite;
            platoon.x = ((600 - ((MAX_BOSS_BUGS * kSpriteScale) + ((MAX_BOSS_BUGS - 1) * kBugSpacing))) / 2);
            platoon.x += i * (kSpriteScale + kBugSpacing);
            bugs[i].dive_path = Bug::O_PATTERN;
            if (i < 2) bugs[i].dive_side = Bug::LEFT;
            else bugs[i].dive_side = Bug::RIGHT;
            bugs[i].GetSound("../resources/Boss_Stricken#2.wav");
        }
        //  RED bugs in row 2 & 3
        if (i >= MAX_BOSS_BUGS && i < (MAX_BOSS_BUGS + MAX_RED_BUGS)) {
            bugs[i].shield = false;
            bugs[i].value = RED_SCORE;
            bugs[i].sprite = red_sprite;
            if ((i - MAX_BOSS_BUGS) < (MAX_RED_BUGS / 2)) {
                platoon.x = ((600 - (((MAX_RED_BUGS / 2) * kSpriteScale) + (((MAX_RED_BUGS / 2) - 1) * kBugSpacing))) / 2);
                platoon.x += (i - MAX_BOSS_BUGS) * (kSpriteScale + kBugSpacing);
                row = 1;
            } else {
                platoon.x = ((600 - (((MAX_RED_BUGS / 2) * kSpriteScale) + (((MAX_RED_BUGS / 2) - 1) * kBugSpacing))) / 2);
                platoon.x += (i - (MAX_BOSS_BUGS + (MAX_RED_BUGS / 2))) * (kSpriteScale + kBugSpacing);
                row = 2;
            }

            bugs[i].dive_path = Bug::S_PATTERN;
            if (((i > 3) && (i < 8)) || ((i > 11) && (i < 16))) bugs[i].dive_side = Bug::LEFT;
            else bugs[i].dive_side = Bug::RIGHT;
            bugs[i].GetSound("../resources/Goei_Stricken.wav");
        }
        // YELLOW bugs in row 3 & 4
        if (i >= (MAX_BOSS_BUGS + MAX_RED_BUGS)) {
            bugs[i].shield = false;
            bugs[i].value = YELLOW_SCORE;
            bugs[i].sprite = yellow_sprite;
            if ((i - (MAX_BOSS_BUGS + MAX_RED_BUGS)) < (MAX_YELLOW_BUGS / 2)) {
                platoon.x = ((600 - (((MAX_YELLOW_BUGS / 2) * kSpriteScale) + (((MAX_YELLOW_BUGS / 2) - 1) * kBugSpacing))) / 2);
                platoon.x += (i - (MAX_BOSS_BUGS + MAX_RED_BUGS)) * (kSpriteScale + kBugSpacing);
                row = 3;
            } else {
                platoon.x = ((600 - (((MAX_YELLOW_BUGS / 2) * kSpriteScale) + (((MAX_YELLOW_BUGS / 2) - 1) * kBugSpacing))) / 2);
                platoon.x += (i - (MAX_BOSS_BUGS + MAX_RED_BUGS + (MAX_YELLOW_BUGS / 2))) * (kSpriteScale + kBugSpacing);
                row = 4;
            }
            bugs[i].dive_path = Bug::J_PATTERN;
            if (((i > 19) && (i < 25)) || ((i > 29) && (i < 35))) bugs[i].dive_side = Bug::LEFT;
            else bugs[i].dive_side = Bug::RIGHT;
            bugs[i].GetSound("../resources/Zako_Stricken.wav");
        }
        platoon.y = kBugPlatoon_y + (row * kSpriteScale);           // 'y' position based on row
        bugs[i].Pos(platoon);                                       // Set the bug's position
        bugs[i].home = platoon;                                     // Store that position as the home spot
        bugs[i].angle = 0;
        bugs[i].attack = false;
        bugs[i].dive_pos = 0;
        bugs[i].missile.sprite = down_missile_sprite;
        bugs[i].missile.Active(false);
    }
}
