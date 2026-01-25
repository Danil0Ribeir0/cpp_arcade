#include "Snake.h"

namespace Snake {

    void Game::Init() {
        body.clear();
        int startX = CELL_COUNT / 2;
        int startY = CELL_COUNT / 2;

        body.push_front({startX - 2, startY});
        body.push_front({startX - 1, startY});
        body.push_front({startX, startY});

        direction = {1, 0};
        running = true;
        paused = false;
        score = 0;
        canTurn = true;
        lastMoveTime = GetTime();
        food = GenerateFood();
    }

    Point Game::GenerateFood() {
        Point p;
        while (true) {
            p = {GetRandomValue(0, CELL_COUNT - 1), GetRandomValue(0, CELL_COUNT - 1)};
            bool onSnake = false;
            for (const auto& segment : body) {
                if (segment == p) {
                    onSnake = true;
                    break;
                }
            }
            if (!onSnake) return p;
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
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        const char* title = "SNAKE ARCADE";
        int titleW = MeasureText(title, 40);
        DrawText(title, screenWidth/2 - titleW/2, 150, 40, DARKGREEN);

        if (DrawButton("INICIAR", {(float)screenWidth/2 - 100, 300, 200, 50})) {
            Init();
            currentScreen = Screen::GAME;
        }

        if (DrawButton("SAIR", {(float)screenWidth/2 - 100, 380, 200, 50})) {
            exitGame = true;
        }

        const char* instructions = "Use Setas ou WASD para mover";
        DrawText(instructions, screenWidth/2 - MeasureText(instructions, 20)/2, screenHeight - 50, 20, LIGHTGRAY);
    }

    void Game::HandleInput() {
        if (currentScreen == Screen::MENU) return;

        if (!running) {
            if (IsKeyPressed(KEY_R)) Init();
            if (IsKeyPressed(KEY_M)) currentScreen = Screen::MENU;
            return;
        }

        if (IsKeyPressed(KEY_P)) paused = !paused;
        if (paused) return;

        if (!canTurn) return;

        int newDirX = direction.x;
        int newDirY = direction.y;
        bool inputDetected = false;

        if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && direction.y == 0) {
            newDirX = 0; newDirY = -1; inputDetected = true;
        }
        else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && direction.y == 0) {
            newDirX = 0; newDirY = 1; inputDetected = true;
        }
        else if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && direction.x == 0) {
            newDirX = -1; newDirY = 0; inputDetected = true;
        }
        else if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && direction.x == 0) {
            newDirX = 1; newDirY = 0; inputDetected = true;
        }

        if (inputDetected) {
            direction = {newDirX, newDirY};
            canTurn = false;
        }
    }

    void Game::Update() {
        if (currentScreen == Screen::MENU) return;
        if (!running || paused) return;

        double currentTime = GetTime();
        if (currentTime - lastMoveTime >= MOVE_INTERVAL) {
            lastMoveTime = currentTime;
            canTurn = true;

            Point newHead = {body.front().x + direction.x, body.front().y + direction.y};

            if (newHead.x < 0 || newHead.x >= CELL_COUNT || newHead.y < 0 || newHead.y >= CELL_COUNT) {
                running = false;
                return;
            }

            for (size_t i = 0; i < body.size() - 1; i++) {
                if (body[i] == newHead) {
                    running = false;
                    return;
                }
            }

            body.push_front(newHead);

            if (newHead == food) {
                score++;
                food = GenerateFood();
            } else {
                body.pop_back();
            }
        }
    }

    void Game::Draw() {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == Screen::MENU) {
            DrawMenu();
        }
        else {
            DrawRectangle(0, 0, GetScreenWidth(), HEADER_HEIGHT, DARKGRAY);
            DrawText(TextFormat("PLACAR: %02i", score), 20, 15, 30, WHITE);
            DrawText("P: Pausa", GetScreenWidth() - 150, 20, 20, LIGHTGRAY);

            int offsetY = HEADER_HEIGHT;

            for (int i = 0; i <= CELL_COUNT; i++) {
                DrawLine(i * CELL_SIZE, offsetY, i * CELL_SIZE, offsetY + CELL_COUNT * CELL_SIZE, Fade(LIGHTGRAY, 0.5f));
                DrawLine(0, offsetY + i * CELL_SIZE, CELL_COUNT * CELL_SIZE, offsetY + i * CELL_SIZE, Fade(LIGHTGRAY, 0.5f));
            }

            DrawRectangle(food.x * CELL_SIZE, food.y * CELL_SIZE + offsetY, CELL_SIZE, CELL_SIZE, RED);

            for (size_t i = 0; i < body.size(); i++) {
                Color c = (i == 0) ? DARKGREEN : LIME;
                DrawRectangle(body[i].x * CELL_SIZE, body[i].y * CELL_SIZE + offsetY, CELL_SIZE, CELL_SIZE, c);
                DrawRectangleLines(body[i].x * CELL_SIZE, body[i].y * CELL_SIZE + offsetY, CELL_SIZE, CELL_SIZE, DARKGRAY);
            }

            if (paused && running) {
                DrawRectangle(0, offsetY, GetScreenWidth(), GetScreenHeight() - offsetY, Fade(BLACK, 0.3f));
                DrawText("PAUSADO", GetScreenWidth()/2 - MeasureText("PAUSADO", 40)/2, offsetY + 300, 40, WHITE);
            }

            if (!running) {
                DrawRectangle(0, offsetY + CELL_COUNT * CELL_SIZE / 2 - 60, CELL_COUNT * CELL_SIZE, 120, Fade(BLACK, 0.8f));
                DrawText("GAME OVER", 280, offsetY + 330, 40, RED);
                DrawText("[R] Reiniciar  [M] Menu", 250, offsetY + 380, 20, WHITE);
            }
        }

        EndDrawing();
    }

    void Game::RunLoop() {
        int screenWidth = CELL_COUNT * CELL_SIZE;
        int screenHeight = (CELL_COUNT * CELL_SIZE) + HEADER_HEIGHT;

        InitWindow(screenWidth, screenHeight, "Snake - C++ Arcade");
        SetTargetFPS(60);

        currentScreen = Screen::MENU;
        exitGame = false;

        while (!WindowShouldClose() && !exitGame) {
            HandleInput();
            Update();
            Draw();
        }
        CloseWindow();
    }

    void run() {
        Game snakeGame;
        snakeGame.RunLoop();
    }
}