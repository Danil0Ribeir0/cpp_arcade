#ifndef CPP_ARCADE_SNAKE_H
#define CPP_ARCADE_SNAKE_H

#include <deque>
#include "raylib.h"

namespace Snake {

    void run();

    struct Point {
        int x, y;
        bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    };

    class Game {
    public:
        void RunLoop();

    private:
        void Init();
        void HandleInput();
        void Update();
        void Draw();
        void DrawMenu();
        bool DrawButton(const char* text, Rectangle rect);

        Point GenerateFood();

        static const int CELL_SIZE = 30;
        static const int CELL_COUNT = 25;
        static const int HEADER_HEIGHT = 60;
        static constexpr double MOVE_INTERVAL = 0.15;

        enum class Screen { MENU, GAME };
        Screen currentScreen = Screen::MENU;
        bool exitGame = false;

        std::deque<Point> body;
        Point direction = {1, 0};
        Point food = {0, 0};
        bool running = false;
        bool paused = false;
        int score = 0;

        double lastMoveTime = 0;
        bool canTurn = true;
    };
}

#endif //CPP_ARCADE_SNAKE_H