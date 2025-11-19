#include "TicTacToe.h"
#include "raylib.h"
#include <vector>
#include <algorithm>

namespace TicTacToe {
    const int SCREEN_SIZE = 600;
    const int CELL_SIZE = SCREEN_SIZE / 3;

    enum class Cell { EMPTY, X, O };
    enum class GameState { PLAYING, WIN_X, WIN_O, DRAW };
    enum class Screen { MENU, DIFFICULTY_SELECT, GAME };
    enum class Opponent { HUMAN, AI };
    enum class Difficulty { EASY, MEDIUM, HARD };

    Cell board[3][3];
    Cell currentPlayer;
    GameState currentState;
    Screen currentScreen = Screen::MENU;
    Opponent selectedOpponent;
    Difficulty aiDifficulty;

    void InitGame() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                board[i][j] = Cell::EMPTY;

        currentPlayer = Cell::X;
        currentState = GameState::PLAYING;
    }

    void CheckWinner() {
        for (int i = 0; i < 3; i++) {
            // Linhas
            if (board[i][0] != Cell::EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                currentState = (board[i][0] == Cell::X) ? GameState::WIN_X : GameState::WIN_O;
                return;
            }
            // Colunas
            if (board[0][i] != Cell::EMPTY && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
                currentState = (board[0][i] == Cell::X) ? GameState::WIN_X : GameState::WIN_O;
                return;
            }
        }

        // Diagonais
        if (board[0][0] != Cell::EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            currentState = (board[0][0] == Cell::X) ? GameState::WIN_X : GameState::WIN_O;
            return;
        }
        if (board[0][2] != Cell::EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            currentState = (board[0][2] == Cell::X) ? GameState::WIN_X : GameState::WIN_O;
            return;
        }

        // Verifica Empate (Velha)
        bool full = true;
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                if(board[i][j] == Cell::EMPTY) full = false;

        if (full) currentState = GameState::DRAW;
    }

    void run() {
        InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Tic-Tac-Toe - C++ Arcade");
        InitGame();

        while (!WindowShouldClose()) {
            // --- 1. UPDATE (Lógica) ---
            if (currentState == GameState::PLAYING) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    int mouseX = GetMouseX();
                    int mouseY = GetMouseY();

                    // Matemática simples para pegar o índice da matriz
                    int col = mouseX / CELL_SIZE;
                    int row = mouseY / CELL_SIZE;

                    // Proteção: verifica limites e se célula está vazia
                    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == Cell::EMPTY) {
                        board[row][col] = currentPlayer;
                        CheckWinner();
                        // Troca turno
                        if (currentState == GameState::PLAYING) {
                            currentPlayer = (currentPlayer == Cell::X) ? Cell::O : Cell::X;
                        }
                    }
                }
            } else {
                // Se o jogo acabou, R para reiniciar
                if (IsKeyPressed(KEY_R)) {
                    InitGame();
                }
            }

            // --- 2. DRAW (Desenho) ---
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Desenha o Grid
            for (int i = 1; i < 3; i++) {
                DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_SIZE, LIGHTGRAY); // Verticais
                DrawLine(0, i * CELL_SIZE, SCREEN_SIZE, i * CELL_SIZE, LIGHTGRAY); // Horizontais
            }

            // Desenha as Peças (Reutilização via Loop)
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    int centerX = col * CELL_SIZE + CELL_SIZE / 2;
                    int centerY = row * CELL_SIZE + CELL_SIZE / 2;

                    if (board[row][col] == Cell::X) {
                        // Desenha X
                        DrawLineEx({(float)centerX - 40, (float)centerY - 40},
                                   {(float)centerX + 40, (float)centerY + 40}, 10, RED);
                        DrawLineEx({(float)centerX + 40, (float)centerY - 40},
                                   {(float)centerX - 40, (float)centerY + 40}, 10, RED);
                    } else if (board[row][col] == Cell::O) {
                        // Desenha O
                        DrawCircleLines(centerX, centerY, 45, BLUE);
                    }
                }
            }

            // UI de Fim de Jogo
            if (currentState != GameState::PLAYING) {
                DrawRectangle(0, SCREEN_SIZE/2 - 50, SCREEN_SIZE, 100, Fade(BLACK, 0.8f));
                const char* text = "";
                if (currentState == GameState::WIN_X) text = "JOGADOR X VENCEU!";
                else if (currentState == GameState::WIN_O) text = "JOGADOR O VENCEU!";
                else text = "EMPATE (VELHA)!";

                int textWidth = MeasureText(text, 30);
                DrawText(text, SCREEN_SIZE/2 - textWidth/2, SCREEN_SIZE/2 - 15, 30, WHITE);
                DrawText("Pressione 'R' para reiniciar", SCREEN_SIZE/2 - 100, SCREEN_SIZE/2 + 20, 20, LIGHTGRAY);
            }

            EndDrawing();
        }

        CloseWindow();
    }
}