#include "Pong.h"
#include "raylib.h"
#include <string>

namespace Pong {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;
    const int PADDLE_WIDTH = 15;
    const int PADDLE_HEIGHT = 80;

    enum class Screen { MENU, GAME };
    Screen currentScreen = Screen::MENU;
    bool exitGame = false;

    struct Ball {
        float x, y;
        float speedX, speedY;
        float radius;

        void Draw() {
            DrawCircle((int)x, (int)y, radius, WHITE);
        }
    };

    struct Paddle {
        float x, y;
        float speed;
        int score;

        void Draw() {
            DrawRectangle((int)x, (int)y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
        }

        Rectangle GetRect() {
            return Rectangle{ x, y, (float)PADDLE_WIDTH, (float)PADDLE_HEIGHT };
        }
    };

    Ball ball;
    Paddle player;
    Paddle cpu;
    bool paused = false;

    void ResetBall() {
        ball.x = SCREEN_WIDTH / 2.0f;
        ball.y = SCREEN_HEIGHT / 2.0f;

        int directionX = GetRandomValue(0, 1) == 0 ? -1 : 1;
        int directionY = GetRandomValue(0, 1) == 0 ? -1 : 1;

        ball.speedX = 5.0f * directionX;
        ball.speedY = 5.0f * directionY;
    }

    void InitPong() {
        ball.radius = 10;
        ResetBall();

        player.x = 20;
        player.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
        player.speed = 6.0f;
        player.score = 0;

        cpu.x = SCREEN_WIDTH - 20 - PADDLE_WIDTH;
        cpu.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
        cpu.speed = 5.5f;
        cpu.score = 0;

        paused = false;
    }

    bool DrawButton(const char* text, Rectangle rect) {
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

    void DrawMenu() {
        int titleW = MeasureText("PONG", 60);
        DrawText("PONG", SCREEN_WIDTH/2 - titleW/2, 80, 60, WHITE);

        if (DrawButton("INICIAR", {(float)SCREEN_WIDTH/2 - 100, 200, 200, 50})) {
            InitPong();
            currentScreen = Screen::GAME;
        }

        if (DrawButton("SAIR", {(float)SCREEN_WIDTH/2 - 100, 280, 200, 50})) {
            exitGame = true;
        }

        const char* inst = "W/S ou Setas para Mover";
        DrawText(inst, SCREEN_WIDTH/2 - MeasureText(inst, 20)/2, SCREEN_HEIGHT - 50, 20, LIGHTGRAY);
    }

    void UpdateGame() {
        if (currentScreen == Screen::MENU) return;

        if (IsKeyPressed(KEY_P)) paused = !paused;
        if (IsKeyPressed(KEY_M)) currentScreen = Screen::MENU;
        if (paused) return;

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            player.y -= player.speed;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            player.y += player.speed;
        }

        if (player.y < 0) player.y = 0;
        if (player.y + PADDLE_HEIGHT > SCREEN_HEIGHT) player.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

        if (ball.y < cpu.y + PADDLE_HEIGHT / 2) {
            cpu.y -= cpu.speed;
        }
        if (ball.y > cpu.y + PADDLE_HEIGHT / 2) {
            cpu.y += cpu.speed;
        }
        if (cpu.y < 0) cpu.y = 0;
        if (cpu.y + PADDLE_HEIGHT > SCREEN_HEIGHT) cpu.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

        ball.x += ball.speedX;
        ball.y += ball.speedY;

        if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= SCREEN_HEIGHT) {
            ball.speedY *= -1;
        }

        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, player.GetRect())) {
            if (ball.speedX < 0) {
                ball.speedX *= -1.1f;
                ball.x = player.x + PADDLE_WIDTH + ball.radius + 1;
            }
        }

        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, cpu.GetRect())) {
            if (ball.speedX > 0) {
                ball.speedX *= -1.1f;
                ball.x = cpu.x - ball.radius - 1;
            }
        }

        if (ball.x < 0) {
            cpu.score++;
            ResetBall();
        }
        if (ball.x > SCREEN_WIDTH) {
            player.score++;
            ResetBall();
        }
    }

    void DrawGame() {
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen == Screen::MENU) {
            DrawMenu();
        } else {
            DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, Fade(WHITE, 0.2f));

            ball.Draw();
            player.Draw();
            cpu.Draw();

            DrawText(std::to_string(player.score).c_str(), SCREEN_WIDTH / 4, 20, 60, WHITE);
            DrawText(std::to_string(cpu.score).c_str(), 3 * SCREEN_WIDTH / 4, 20, 60, WHITE);

            if (paused) DrawText("PAUSED", SCREEN_WIDTH/2 - MeasureText("PAUSED", 30)/2, SCREEN_HEIGHT/2, 30, GRAY);

            DrawText("[M] Menu  [P] Pausa", 10, SCREEN_HEIGHT - 20, 10, GRAY);
        }

        EndDrawing();
    }

    void run() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong - C++ Arcade");
        SetTargetFPS(60);

        InitPong();
        currentScreen = Screen::MENU;
        exitGame = false;

        while (!WindowShouldClose() && !exitGame) {
            UpdateGame();
            DrawGame();
        }

        CloseWindow();
    }
}