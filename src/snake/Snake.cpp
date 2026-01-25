#include "Snake.h"
#include "raylib.h"
#include <deque> // Importante: Fila de duas pontas

namespace Snake {

    const int CELL_SIZE = 30;
    const int CELL_COUNT = 25;
    const int SCREEN_SIZE = CELL_SIZE * CELL_COUNT;
    const double INITIAL_SPEED = 0.2;

    struct Point {
        int x, y;
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    std::deque<Point> body;
    Point direction = {1, 0}; // Começa movendo para direita
    Point food;
    bool running = true;
    int score = 0;
    double lastUpdateTime = 0;

    // --- FUNÇÕES AUXILIARES ---

    // Gera comida em lugar aleatório (que não seja em cima da cobra)
    Point GenerateFood() {
        Point p;
        while (true) {
            p = {GetRandomValue(0, CELL_COUNT - 1), GetRandomValue(0, CELL_COUNT - 1)};
            // Verifica se spawnou em cima da cobra
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

    void InitSnake() {
        body.clear();
        // Cobra começa com 3 segmentos
        body.push_front({6, 9});
        body.push_front({5, 9});
        body.push_front({4, 9});

        direction = {1, 0};
        running = true;
        score = 0;
        food = GenerateFood();
    }

    // Controle de Tempo (Tick Rate)
    bool EventTriggered(double interval) {
        double currentTime = GetTime();
        if (currentTime - lastUpdateTime >= interval) {
            lastUpdateTime = currentTime;
            return true;
        }
        return false;
    }

    void Update() {
        if (!running) {
            if (IsKeyPressed(KEY_R)) InitSnake();
            return;
        }

        // 1. Input (Impede virada de 180 graus)
        if (IsKeyPressed(KEY_UP) && direction.y != 1) direction = {0, -1};
        if (IsKeyPressed(KEY_DOWN) && direction.y != -1) direction = {0, 1};
        if (IsKeyPressed(KEY_LEFT) && direction.x != 1) direction = {-1, 0};
        if (IsKeyPressed(KEY_RIGHT) && direction.x != -1) direction = {1, 0};

        // 2. Movimento (Só acontece a cada X segundos)
        if (EventTriggered(INITIAL_SPEED)) {
            // Calcula onde será a nova cabeça
            Point newHead = {body[0].x + direction.x, body[0].y + direction.y};

            // A. Colisão com Paredes
            if (newHead.x < 0 || newHead.x >= CELL_COUNT || newHead.y < 0 || newHead.y >= CELL_COUNT) {
                running = false;
                return;
            }

            // B. Colisão com o Próprio Corpo
            for (const auto& segment : body) {
                if (newHead == segment) {
                    running = false;
                    return;
                }
            }

            // Adiciona nova cabeça
            body.push_front(newHead);

            // C. Comer Comida
            if (newHead == food) {
                score++;
                food = GenerateFood();
                // Não fazemos pop_back(), então ela cresce
            } else {
                // Se não comeu, remove a cauda para manter o tamanho
                body.pop_back();
            }
        }
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(RAYWHITE); // Fundo claro dessa vez

        // Desenha Comida
        DrawRectangle(food.x * CELL_SIZE, food.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);

        // Desenha Cobra
        for (size_t i = 0; i < body.size(); i++) {
            // A cabeça é verde escuro, o corpo verde claro
            Color c = (i == 0) ? DARKGREEN : GREEN;
            DrawRectangle(body[i].x * CELL_SIZE, body[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, c);
            // Borda para ver os segmentos
            DrawRectangleLines(body[i].x * CELL_SIZE, body[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
        }

        // UI
        DrawText(TextFormat("Score: %i", score), 10, 10, 20, DARKGRAY);

        if (!running) {
            DrawRectangle(0, SCREEN_SIZE/2 - 50, SCREEN_SIZE, 100, Fade(BLACK, 0.8f));
            DrawText("GAME OVER", SCREEN_SIZE/2 - MeasureText("GAME OVER", 40)/2, SCREEN_SIZE/2 - 20, 40, RED);
            DrawText("Pressione [R] para Reiniciar", SCREEN_SIZE/2 - MeasureText("Pressione [R] para Reiniciar", 20)/2, SCREEN_SIZE/2 + 30, 20, WHITE);
        }

        EndDrawing();
    }

    void run() {
        InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Snake - C++ Arcade");
        SetTargetFPS(60); // O jogo roda a 60FPS para inputs suaves, mas a cobra move a 5FPS
        InitSnake();

        while (!WindowShouldClose()) {
            Update();
            Draw();
        }
        CloseWindow();
    }
}