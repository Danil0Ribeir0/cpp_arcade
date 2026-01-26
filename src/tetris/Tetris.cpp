#include "Tetris.h"
#include <cstring>

namespace Tetris {

    const Tetromino SHAPES[7] = {
        { {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, SKYBLUE, 4 },
        { {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, BLUE, 3 },
        { {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, ORANGE, 3 },
        { {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, YELLOW, 2 },
        { {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, GREEN, 3 },
        { {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, PURPLE, 3 },
        { {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}, RED, 3 }
    };

    void Game::Init() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                grid[i][j] = BLACK;
            }
        }

        score = 0;
        linesCleared = 0;
        running = true;
        paused = false;
        fallInterval = 0.5;

        nextPiece = SHAPES[GetRandomValue(0, 6)];
        NewPiece();
    }

    void Game::NewPiece() {
        currentPiece = nextPiece;
        nextPiece = SHAPES[GetRandomValue(0, 6)];

        currentPos.x = COLS / 2 - currentPiece.size / 2;
        currentPos.y = 0;

        if (!CheckCollision(0, 0, currentPiece.shape)) {
            running = false;
        }
    }

    bool Game::CheckCollision(int offsetx, int offsety, int shape[4][4]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (shape[i][j]) {
                    int newX = currentPos.x + j + offsetx;
                    int newY = currentPos.y + i + offsety;

                    if (newX < 0 || newX >= COLS || newY >= ROWS) return false;
                    if (newY >= 0 && grid[newY][newX].a != 0 && !(grid[newY][newX].r == 0 && grid[newY][newX].g == 0 && grid[newY][newX].b == 0)) return false;
                }
            }
        }
        return true;
    }

    void Game::RotatePiece() {
        int temp[4][4] = {0}; // Inicializado com zeros para evitar lixo de memória
        int n = currentPiece.size;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                temp[j][n - 1 - i] = currentPiece.shape[i][j];
            }
        }

        if (CheckCollision(0, 0, temp)) {
            memcpy(currentPiece.shape, temp, sizeof(temp));
        } else {
            if (CheckCollision(1, 0, temp)) {
                currentPos.x++;
                memcpy(currentPiece.shape, temp, sizeof(temp));
            } else if (CheckCollision(-1, 0, temp)) {
                currentPos.x--;
                memcpy(currentPiece.shape, temp, sizeof(temp));
            }
        }
    }

    void Game::LockPiece() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (currentPiece.shape[i][j]) {
                    int r = currentPos.y + i;
                    int c = currentPos.x + j;
                    if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                        grid[r][c] = currentPiece.color;
                    }
                }
            }
        }
        ClearLines();
        NewPiece();
    }

    void Game::ClearLines() {
        int lines = 0;
        for (int i = ROWS - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < COLS; j++) {
                Color c = grid[i][j];
                if (c.r == 0 && c.g == 0 && c.b == 0) {
                    full = false;
                    break;
                }
            }

            if (full) {
                lines++;
                for (int k = i; k > 0; k--) {
                    for (int j = 0; j < COLS; j++) {
                        grid[k][j] = grid[k - 1][j];
                    }
                }
                for (int j = 0; j < COLS; j++) grid[0][j] = BLACK;
                i++;
            }
        }

        if (lines > 0) {
            score += lines * 100 * lines;
            linesCleared += lines;
            if (linesCleared % 5 == 0 && fallInterval > 0.1) fallInterval -= 0.05;
        }
    }

    bool Game::DrawButton(const char* text, Rectangle rect) {
        bool clicked = false;
        Vector2 mousePoint = GetMousePosition();
        bool hover = CheckCollisionPointRec(mousePoint, rect);

        DrawRectangleRec(rect, hover ? DARKGRAY : GRAY);
        DrawRectangleLinesEx(rect, 2, BLACK);

        int textW = MeasureText(text, 20);
        DrawText(text, rect.x + rect.width/2 - textW/2, rect.y + rect.height/2 - 10, 20, WHITE);

        if (hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) clicked = true;
        return clicked;
    }

    void Game::DrawMenu() {
        int titleW = MeasureText("TETRIS", 50);
        DrawText("TETRIS", SCREEN_WIDTH/2 - titleW/2, 150, 50, PURPLE);

        if (DrawButton("INICIAR", {(float)SCREEN_WIDTH/2 - 100, 300, 200, 50})) {
            Init();
            currentScreen = Screen::GAME;
        }

        if (DrawButton("SAIR", {(float)SCREEN_WIDTH/2 - 100, 380, 200, 50})) {
            exitGame = true;
        }

        const char* inst = "Setas: Mover/Girar | Baixo: Acelerar";
        DrawText(inst, SCREEN_WIDTH/2 - MeasureText(inst, 20)/2, SCREEN_HEIGHT - 50, 20, LIGHTGRAY);
    }

    void Game::Update() {
        if (currentScreen == Screen::MENU) return;

        if (!running) {
            if (IsKeyPressed(KEY_R)) Init();
            if (IsKeyPressed(KEY_M)) currentScreen = Screen::MENU;
            return;
        }

        if (IsKeyPressed(KEY_P)) paused = !paused;
        if (paused) return;

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            if (CheckCollision(-1, 0, currentPiece.shape)) currentPos.x--;
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            if (CheckCollision(1, 0, currentPiece.shape)) currentPos.x++;
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            RotatePiece();
        }

        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            if (GetTime() - lastFallTime > 0.05) {
                if (CheckCollision(0, 1, currentPiece.shape)) {
                    currentPos.y++;
                    lastFallTime = GetTime();
                    score++;
                } else {
                    LockPiece();
                }
            }
        }
        else if (GetTime() - lastFallTime > fallInterval) {
            if (CheckCollision(0, 1, currentPiece.shape)) {
                currentPos.y++;
            } else {
                LockPiece();
            }
            lastFallTime = GetTime();
        }
    }

    void Game::Draw() {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (currentScreen == Screen::MENU) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RAYWHITE);
            DrawMenu();
        } else {
            DrawRectangle(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, BLACK);
            DrawText(TextFormat("SCORE: %05i", score), 20, 20, 20, WHITE);

            DrawRectangle(0, HEADER_HEIGHT, COLS * BLOCK_SIZE, ROWS * BLOCK_SIZE, BLACK);

            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (grid[i][j].r != 0 || grid[i][j].g != 0 || grid[i][j].b != 0) {
                        DrawRectangle(j * BLOCK_SIZE, i * BLOCK_SIZE + HEADER_HEIGHT, BLOCK_SIZE - 1, BLOCK_SIZE - 1, grid[i][j]);
                    }
                    DrawRectangleLines(j * BLOCK_SIZE, i * BLOCK_SIZE + HEADER_HEIGHT, BLOCK_SIZE, BLOCK_SIZE, Fade(DARKGRAY, 0.2f));
                }
            }

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (currentPiece.shape[i][j]) {
                        int drawX = (currentPos.x + j) * BLOCK_SIZE;
                        int drawY = (currentPos.y + i) * BLOCK_SIZE + HEADER_HEIGHT;
                        if (drawY >= HEADER_HEIGHT) {
                            DrawRectangle(drawX, drawY, BLOCK_SIZE - 1, BLOCK_SIZE - 1, currentPiece.color);
                        }
                    }
                }
            }

            int sideX = COLS * BLOCK_SIZE;
            DrawRectangle(sideX, HEADER_HEIGHT, SIDE_PANEL_WIDTH, SCREEN_HEIGHT - HEADER_HEIGHT, DARKBLUE);
            DrawText("NEXT", sideX + 70, HEADER_HEIGHT + 30, 20, WHITE);
            
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (nextPiece.shape[i][j]) {
                        DrawRectangle(sideX + 50 + j * BLOCK_SIZE, HEADER_HEIGHT + 70 + i * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, nextPiece.color);
                    }
                }
            }

            DrawText(TextFormat("LINES: %i", linesCleared), sideX + 50, HEADER_HEIGHT + 250, 20, WHITE);

            if (paused) DrawText("PAUSED", COLS * BLOCK_SIZE / 2 - 40, SCREEN_HEIGHT/2, 30, WHITE);

            if (!running) {
                DrawRectangle(0, SCREEN_HEIGHT/2 - 60, SCREEN_WIDTH, 120, Fade(BLACK, 0.9f));
                DrawText("GAME OVER", SCREEN_WIDTH/2 - MeasureText("GAME OVER", 40)/2, SCREEN_HEIGHT/2 - 20, 40, RED);
                DrawText("[R] Reiniciar  [M] Menu", SCREEN_WIDTH/2 - 110, SCREEN_HEIGHT/2 + 30, 20, WHITE);
            }
        }

        EndDrawing();
    }

    void Game::RunLoop() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris - C++ Arcade");
        SetTargetFPS(60);
        Init();
        currentScreen = Screen::MENU;

        while (!WindowShouldClose() && !exitGame) {
            Update();
            Draw();
        }
        CloseWindow();
    }

    void run() {
        Game game;
        game.RunLoop();
    }
}