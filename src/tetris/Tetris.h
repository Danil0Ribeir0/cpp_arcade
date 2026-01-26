#ifndef CPP_ARCADE_TETRIS_H
#define CPP_ARCADE_TETRIS_H

#include "raylib.h"
#include <vector>

namespace Tetris {

    void run();

    struct Point {
        int x, y;
    };

    struct Tetromino {
        int shape[4][4];
        Color color;
        int size;
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

        void NewPiece();
        bool CheckCollision(int dirX, int dirY, int rotationState[4][4]);
        void LockPiece();
        void ClearLines();
        void RotatePiece();

        static const int COLS = 10;
        static const int ROWS = 20;
        static const int BLOCK_SIZE = 30;
        static const int HEADER_HEIGHT = 60;
        static const int SIDE_PANEL_WIDTH = 200;

        static const int SCREEN_WIDTH = (COLS * BLOCK_SIZE) + SIDE_PANEL_WIDTH;
        static const int SCREEN_HEIGHT = (ROWS * BLOCK_SIZE) + HEADER_HEIGHT;

        enum class Screen { MENU, GAME };
        Screen currentScreen = Screen::MENU;
        bool exitGame = false;

        Color grid[ROWS][COLS];
        bool running = false;
        bool paused = false;
        int score = 0;
        int linesCleared = 0;

        Tetromino currentPiece;
        Tetromino nextPiece;
        Point currentPos;

        double lastFallTime = 0;
        double fallInterval = 0.5;
    };
}

#endif //CPP_ARCADE_TETRIS_H