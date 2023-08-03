#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <iostream>
#include "SDL.h"

class Bug {
    public:
        Bug() {}
        ~Bug() {}

        SDL_Rect GetRect() const;                       // Get the Rect of sprite
        void Hit();
        bool Active();                                  // Return if bug is alive/active
        void Attack();                                  // Attack mode for bug

        float speed{4.0f};
        bool alive{true};
        int x;                                          // Current position on screed
        int y;
        int w = kSpriteScale;                           // size of sprite for screen
        int h = kSpriteScale;
        int platoon_x;                                  // Position in platoon formation
        int platoon_y;
        int hit{0};                                     // Keep track of the die process
        int value;
        bool shield;                                    // Boss bugs take 2 hits to kill
        bool attack{false};                             // Bug in attack mode

    private:
        const static std::size_t kSpriteScale{42};      // Scale factor for sprites to game screen
};

class Enemy {
    public:
        Enemy();
        ~Enemy() {}

        void Update();
        void Start();

        float speed{4.0f};

        static const int MAX_BOSS_BUGS = 4;
        static const int MAX_RED_BUGS = 16;
        static const int MAX_YELLOW_BUGS = 20;

        static const int BOSS_SCORE = 150;
        static const int RED_SCORE = 80;
        static const int YELLOW_SCORE = 50;

        Bug boss_bugs[MAX_BOSS_BUGS];
        Bug red_bugs[MAX_RED_BUGS];
        Bug yellow_bugs[MAX_YELLOW_BUGS];

        int hive{0};

    private:
        const static std::size_t kBugPlatoon_y{120};              // Y Position of the bug platoon
        const static std::size_t kBugSpacing{4};                  // Spacing between bug sprites
};

class NewEnemy {
    public:
        NewEnemy() {}
        ~NewEnemy() {}


    private:
};

class Boss : public NewEnemy {
    public:
        Boss();
        ~Boss() {}

        SDL_Rect GetRect() const;                       // Get the Rect of sprite
        void Hit();

        float speed{4.0f};
        bool alive{true};
        int x;
        int y;
        int w = kSpriteScale;
        int h = kSpriteScale;
        int value;

    private:
        const static std::size_t kSpriteScale{42};      // Scale factor for sprites to game screen
};

#endif