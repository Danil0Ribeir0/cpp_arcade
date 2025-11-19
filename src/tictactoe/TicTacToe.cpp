#include "TicTacToe.h"
#include "raylib.h"

namespace TicTacToe {
    void run() {
        // Inicializa a janela deste jogo específico
        InitWindow(800, 600, "Tic-Tac-Toe - C++ Arcade");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("TIC TAC TOE - Funcionando!", 190, 200, 30, DARKGRAY);
            DrawText("Pressione ESC para voltar (Mentira, vai fechar tudo por enquanto)", 50, 300, 20, LIGHTGRAY);
            EndDrawing();
        }

        CloseWindow();
    }
}