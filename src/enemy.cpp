#include "enemy.h"

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

void Bug::Pos(SDL_Point p) {
    x = p.x;
    y = p.y;
}

SDL_Point Bug::Pos() const {
    SDL_Point position;

    position.x = x;
    position.y = y;
    
    return position;
}

bool Bug::Active() {
  return alive;
}

void Bug::Hit() {
    if (shield) {
        sprite = boss_blue_sprite;
        shield = false;
    } else hit = 1;
}

bool Bug::Die() {
    switch (hit) {
        case 1:
            sprite = explode_1_sprite;
            break;
        case 2:
            sprite = explode_2_sprite;
            break;
        case 3:
            sprite = explode_3_sprite;
            break;
        case 4:
            sprite = explode_4_sprite;
            break;
    }
    hit++;
    if (hit > 4) {
        hit = 0;
        alive = false;
        return true;
    } else return false;
}

void Bug::Dive() {
    if (fire) {
        missile.y += 12;;
        if (y > 800) {
            fire = false;
            missile.Active(false);
        }
    }
    else {
        if (dive_pos > 8) {
            fire = true;
            missile.Active(true);
            missile.x = x + 20;
            missile.y = y + 44;
        }
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
            dive_pos++;
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
            dive_pos++;
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
            dive_pos++;
        }
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
};

void Enemy::Update() {
    // Update the bug positions
    int direction;

    for (int i = 0 ; i < HIVE_SIZE; i++) {
        if (bugs[i].alive) {
            if (bugs[i].hit > 0) {
                if (bugs[i].Die()) hive--;
            } else if (bugs[i].attack) bugs[i].Dive();
        }
    }
}

void Enemy::Start() {
    hive = HIVE_SIZE;
    int row = 0;
    int direction;

    // Setup the bug platoon positions
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
        }
        platoon.y = kBugPlatoon_y + (row * kSpriteScale);
        bugs[i].Pos(platoon);
        bugs[i].home = platoon;
        bugs[i].missile.sprite = down_missile_sprite;
    }
}
