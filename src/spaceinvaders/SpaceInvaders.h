#ifndef CPP_ARCADE_SPACEINVADERS_H
#define CPP_ARCADE_SPACEINVADERS_H

#include "raylib.h"
#include <vector>

namespace SpaceInvaders {

    void run();

    struct Entity {
        Rectangle rect;
        bool active;
        Color color;
    };

    struct Bullet {
        Rectangle rect;
        bool active;
        float speed;
        bool playerBullet;
    };

    class Game {
    public:
        void RunLoop();

    private:
        void Init();
        void Update();
        void Draw();
        void DrawMenu();
        bool DrawButton(const char* text, Rectangle rect);
        void FireAlienBullet();

        static const int SCREEN_WIDTH = 800;
        static const int SCREEN_HEIGHT = 600;
        static const int HEADER_HEIGHT = 60;

        enum class Screen { MENU, GAME };
        Screen currentScreen = Screen::MENU;
        bool exitGame = false;

        bool running = false;
        bool paused = false;
        int score = 0;
        int lives = 3;
        int wave = 1;

        Entity player;
        std::vector<Entity> aliens;
        std::vector<Entity> bunkers;
        std::vector<Bullet> bullets;

        int alienDirection = 1;
        float lastMoveTime = 0;
        float moveInterval = 0.5f;
    };
}

#endif //CPP_ARCADE_SPACEINVADERS_H