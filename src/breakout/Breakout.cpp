#include "Breakout.h"
#include <cmath>

namespace Breakout {

    void Game::Init() {
        score = 0;
        lives = 3;
        paused = false;
        running = true;

        paddle = { (float)SCREEN_WIDTH / 2 - 50, (float)SCREEN_HEIGHT - 50, 100, 20 };

        ball.active = false;
        ball.radius = 7.0f;
        ball.position = { paddle.x + paddle.width / 2, paddle.y - ball.radius - 1 };
        ball.speed = { 0, 0 };

        bricks.clear();
        float brickWidth = (float)SCREEN_WIDTH / BRICK_COLS;
        float brickHeight = 30;

        for (int i = 0; i < BRICK_ROWS; i++) {
            for (int j = 0; j < BRICK_COLS; j++) {
                Brick b;
                b.rect = { j * brickWidth, i * brickHeight + HEADER_HEIGHT + 50, brickWidth - 2, brickHeight - 2 };
                b.active = true;
                
                if (i == 0) b.color = RED;
                else if (i == 1) b.color = ORANGE;
                else if (i == 2) b.color = YELLOW;
                else if (i == 3) b.color = GREEN;
                else b.color = BLUE;

                bricks.push_back(b);
            }
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
        int titleW = MeasureText("BREAKOUT", 40);
        DrawText("BREAKOUT", SCREEN_WIDTH/2 - titleW/2, 150, 40, DARKBLUE);

        if (DrawButton("INICIAR", {(float)SCREEN_WIDTH/2 - 100, 300, 200, 50})) {
            Init();
            currentScreen = Screen::GAME;
        }

        if (DrawButton("SAIR", {(float)SCREEN_WIDTH/2 - 100, 380, 200, 50})) {
            exitGame = true;
        }
    }

    void Game::Update() {
        if (currentScreen == Screen::MENU) return;

        if (lives <= 0 || bricks.empty()) {
            running = false;
            if (IsKeyPressed(KEY_R)) Init();
            if (IsKeyPressed(KEY_M)) currentScreen = Screen::MENU;
            return;
        }

        if (IsKeyPressed(KEY_P)) paused = !paused;
        if (paused) return;

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) paddle.x -= 7.0f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) paddle.x += 7.0f;

        if (paddle.x < 0) paddle.x = 0;
        if (paddle.x + paddle.width > SCREEN_WIDTH) paddle.x = SCREEN_WIDTH - paddle.width;

        if (!ball.active) {
            ball.position = { paddle.x + paddle.width / 2, paddle.y - ball.radius - 1 };
            if (IsKeyPressed(KEY_SPACE)) {
                ball.active = true;
                ball.speed = { 5.0f, -5.0f };
            }
        } else {
            ball.position.x += ball.speed.x;
            ball.position.y += ball.speed.y;

            if (ball.position.x - ball.radius < 0 || ball.position.x + ball.radius > SCREEN_WIDTH)
                ball.speed.x *= -1;
            if (ball.position.y - ball.radius < HEADER_HEIGHT) 
                ball.speed.y *= -1;

            if (ball.position.y > SCREEN_HEIGHT) {
                ball.active = false;
                lives--;
            }

            if (CheckCollisionCircleRec(ball.position, ball.radius, paddle)) {
                ball.speed.y *= -1;
                ball.position.y = paddle.y - ball.radius - 1;

                float hitPoint = ball.position.x - (paddle.x + paddle.width / 2);
                ball.speed.x = hitPoint * 0.15f; 
            }

            for (auto& brick : bricks) {
                if (brick.active) {
                    if (CheckCollisionCircleRec(ball.position, ball.radius, brick.rect)) {
                        brick.active = false;
                        ball.speed.y *= -1;
                        score += 10;
                        break; 
                    }
                }
            }
        }
    }

    void Game::Draw() {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == Screen::MENU) {
            DrawMenu();
        } else {
            DrawRectangle(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, DARKGRAY);
            DrawText(TextFormat("SCORE: %03i", score), 20, 20, 20, WHITE);
            DrawText(TextFormat("LIVES: %i", lives), 150, 20, 20, WHITE);

            if (!running) {
                const char* msg = (lives > 0) ? "VICTORY!" : "GAME OVER";
                Color msgColor = (lives > 0) ? GREEN : RED;
                DrawText(msg, SCREEN_WIDTH/2 - MeasureText(msg, 40)/2, SCREEN_HEIGHT/2, 40, msgColor);
                DrawText("[R] Reiniciar  [M] Menu", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 50, 20, GRAY);
            } else {
                if (paused) DrawText("PAUSED", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 30, GRAY);

                DrawCircleV(ball.position, ball.radius, MAROON);

                DrawRectangleRec(paddle, BLACK);

                for (const auto& brick : bricks) {
                    if (brick.active) {
                        DrawRectangleRec(brick.rect, brick.color);
                        DrawRectangleLinesEx(brick.rect, 1, Fade(BLACK, 0.3f));
                    }
                }

                if (!ball.active) {
                    DrawText("Pressione ESPACO para lancar", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 50, 20, DARKGRAY);
                }
            }
        }
        EndDrawing();
    }

    void Game::RunLoop() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout - C++ Arcade");
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