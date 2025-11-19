#include "Pong.h"
#include "raylib.h"
#include <string>

namespace Pong {

    // --- CONSTANTES ---
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;
    const int PADDLE_WIDTH = 15;
    const int PADDLE_HEIGHT = 80;

    // --- ESTRUTURAS DE DADOS (STRUCTS) ---
    // Agrupamos dados relacionados. Em C++, struct é quase uma classe.
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

        // Retorna o retângulo para calculos de colisão
        Rectangle GetRect() {
            return Rectangle{ x, y, (float)PADDLE_WIDTH, (float)PADDLE_HEIGHT };
        }
    };

    // --- VARIÁVEIS GLOBAIS DO JOGO ---
    Ball ball;
    Paddle player;
    Paddle cpu;

    // --- FUNÇÕES AUXILIARES ---

    void ResetBall() {
        ball.x = SCREEN_WIDTH / 2.0f;
        ball.y = SCREEN_HEIGHT / 2.0f;

        // Aleatoriedade na direção inicial (esquerda ou direita)
        int directionX = GetRandomValue(0, 1) == 0 ? -1 : 1;
        int directionY = GetRandomValue(0, 1) == 0 ? -1 : 1;

        ball.speedX = 5.0f * directionX;
        ball.speedY = 5.0f * directionY;
    }

    void InitPong() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong - C++ Arcade");
        SetTargetFPS(60);

        ball.radius = 10;
        ResetBall();

        // Configuração Jogador (Esquerda)
        player.x = 20; // Margem de 20px
        player.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
        player.speed = 6.0f;
        player.score = 0;

        // Configuração CPU (Direita)
        cpu.x = SCREEN_WIDTH - 20 - PADDLE_WIDTH;
        cpu.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
        cpu.speed = 5.5f; // Um pouco mais lento que o humano para ser justo
        cpu.score = 0;
    }

    void UpdateGame() {
        // 1. Movimento do Jogador (W/S ou Setas)
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            player.y -= player.speed;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            player.y += player.speed;
        }

        // Limitar Jogador ao Ecrã (Clamping)
        if (player.y < 0) player.y = 0;
        if (player.y + PADDLE_HEIGHT > SCREEN_HEIGHT) player.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

        // 2. Movimento da CPU (IA Simples)
        // A CPU tenta seguir a posição Y da bola
        if (ball.y < cpu.y + PADDLE_HEIGHT / 2) {
            cpu.y -= cpu.speed;
        }
        if (ball.y > cpu.y + PADDLE_HEIGHT / 2) {
            cpu.y += cpu.speed;
        }
        // Limitar CPU ao Ecrã
        if (cpu.y < 0) cpu.y = 0;
        if (cpu.y + PADDLE_HEIGHT > SCREEN_HEIGHT) cpu.y = SCREEN_HEIGHT - PADDLE_HEIGHT;

        // 3. Movimento da Bola
        ball.x += ball.speedX;
        ball.y += ball.speedY;

        // 4. Colisões da Bola com Paredes (Cima/Baixo)
        if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= SCREEN_HEIGHT) {
            ball.speedY *= -1; // Inverte direção vertical
        }

        // 5. Colisão Bola com Raquetes
        // CheckCollisionCircleRec é uma função nativa da Raylib muito útil
        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, player.GetRect())) {
            if (ball.speedX < 0) { // Só rebate se a bola estiver vindo na direção da raquete
                ball.speedX *= -1.1f; // Aumenta velocidade em 10% a cada batida (emoção!)
                ball.x = player.x + PADDLE_WIDTH + ball.radius + 1; // Descola a bola para evitar bugs
            }
        }

        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, cpu.GetRect())) {
            if (ball.speedX > 0) {
                ball.speedX *= -1.1f;
                ball.x = cpu.x - ball.radius - 1;
            }
        }

        // 6. Pontuação (Bola saiu do ecrã)
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
        ClearBackground(BLACK); // Pong clássico é fundo preto

        // Desenha a linha do meio (rede)
        DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, Fade(WHITE, 0.2f));

        ball.Draw();
        player.Draw();
        cpu.Draw();

        // Desenha Placar
        DrawText(std::to_string(player.score).c_str(), SCREEN_WIDTH / 4, 20, 60, WHITE);
        DrawText(std::to_string(cpu.score).c_str(), 3 * SCREEN_WIDTH / 4, 20, 60, WHITE);

        DrawText("ESC para Sair", 10, SCREEN_HEIGHT - 20, 10, GRAY);

        EndDrawing();
    }

    // --- LOOP PRINCIPAL ---
    void run() {
        InitPong();

        while (!WindowShouldClose()) {
            UpdateGame();
            DrawGame();
        }

        CloseWindow();
    }
}