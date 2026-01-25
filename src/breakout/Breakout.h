#ifndef CPP_ARCADE_BREAKOUT_H
#define CPP_ARCADE_BREAKOUT_H

#include "raylib.h"
#include <vector>

namespace Breakout {

    void run();

    struct Brick {
        Rectangle rect;
        bool active;
        Color color;
    };

    struct Ball {
        Vector2 position;
        Vector2 speed;
        float radius;
        bool active;
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

        static const int SCREEN_WIDTH = 800;
        static const int SCREEN_HEIGHT = 600;
        static const int BRICK_ROWS = 5;
        static const int BRICK_COLS = 10;
        static const int HEADER_HEIGHT = 60;

        enum class Screen { MENU, GAME };
        Screen currentScreen = Screen::MENU;
        bool exitGame = false;

        bool paused = false;
        bool running = false;
        int score = 0;
        int lives = 3;

        Rectangle paddle;
        Ball ball;
        std::vector<Brick> bricks;
    };
}

#endif //CPP_ARCADE_BREAKOUT_H