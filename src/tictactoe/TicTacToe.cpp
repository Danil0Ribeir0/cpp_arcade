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
            if (board[i][0] != Cell::EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                currentState = (board[i][0] == Cell::X) ? GameState::WIN_X : GameState::WIN_O;
                return;
            }
            if (board[0][i] != Cell::EMPTY && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
                currentState = (board[0][i] == Cell::X) ? GameState::WIN_X : GameState::WIN_O;
                return;
            }
        }

        if (board[0][0] != Cell::EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            currentState = (board[0][0] == Cell::X) ? GameState::WIN_X : GameState::WIN_O;
            return;
        }
        if (board[0][2] != Cell::EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            currentState = (board[0][2] == Cell::X) ? GameState::WIN_X : GameState::WIN_O;
            return;
        }

        bool full = true;
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                if(board[i][j] == Cell::EMPTY) full = false;

        if (full) currentState = GameState::DRAW;
    }

    struct Point { int r, c; };

    std::vector<Point> GetEmptyCells() {
        std::vector<Point> empties;
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                if(board[i][j] == Cell::EMPTY) empties.push_back({i, j});
        return empties;
    }

    void AImove_Easy() {
        auto empties = GetEmptyCells();
        if (empties.empty()) return;
        Point p = empties[GetRandomValue(0, empties.size() - 1)];
        board[p.r][p.c] = Cell::O;
    }

    int Minimax(Cell tempBoard[3][3], int depth, bool isMaximizing) {
        for(int i=0; i<3; i++){
             if(tempBoard[i][0] != Cell::EMPTY && tempBoard[i][0]==tempBoard[i][1] && tempBoard[i][1]==tempBoard[i][2]){
                 if(tempBoard[i][0] == Cell::O) return 10 - depth;
                 else return depth - 10;
             }
             if(tempBoard[0][i] != Cell::EMPTY && tempBoard[0][i]==tempBoard[1][i] && tempBoard[1][i]==tempBoard[2][i]){
                 if(tempBoard[0][i] == Cell::O) return 10 - depth;
                 else return depth - 10;
             }
        }
        if(tempBoard[0][0] != Cell::EMPTY && tempBoard[0][0]==tempBoard[1][1] && tempBoard[1][1]==tempBoard[2][2]){
             if(tempBoard[0][0] == Cell::O) return 10 - depth; else return depth - 10;
        }
        if(tempBoard[0][2] != Cell::EMPTY && tempBoard[0][2]==tempBoard[1][1] && tempBoard[1][1]==tempBoard[2][0]){
             if(tempBoard[0][2] == Cell::O) return 10 - depth; else return depth - 10;
        }

        bool full = true;
        for(int i=0; i<3; i++) for(int j=0; j<3; j++) if(tempBoard[i][j] == Cell::EMPTY) full = false;
        if(full) return 0;

        if (isMaximizing) {
            int bestScore = -1000;
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    if(tempBoard[i][j] == Cell::EMPTY){
                        tempBoard[i][j] = Cell::O;
                        int score = Minimax(tempBoard, depth + 1, false);
                        tempBoard[i][j] = Cell::EMPTY; // Backtracking (Desfaz)
                        bestScore = std::max(score, bestScore);
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = 1000;
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    if(tempBoard[i][j] == Cell::EMPTY){
                        tempBoard[i][j] = Cell::X;
                        int score = Minimax(tempBoard, depth + 1, true);
                        tempBoard[i][j] = Cell::EMPTY;
                        bestScore = std::min(score, bestScore);
                    }
                }
            }
            return bestScore;
        }
    }

    void AImove_Hard() {
        int bestScore = -1000;
        Point bestMove = {-1, -1};

        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j] == Cell::EMPTY){
                    board[i][j] = Cell::O;
                    int score = Minimax(board, 0, false);
                    board[i][j] = Cell::EMPTY;

                    if(score > bestScore){
                        bestScore = score;
                        bestMove = {i, j};
                    }
                }
            }
        }
        if(bestMove.r != -1) board[bestMove.r][bestMove.c] = Cell::O;
    }

    void AImove_Medium() {
        if (GetRandomValue(0, 100) < 40) AImove_Easy();
        else AImove_Hard();
    }

    void ExecuteAI() {
        if (currentState != GameState::PLAYING) return;

        if (aiDifficulty == Difficulty::EASY) AImove_Easy();
        else if (aiDifficulty == Difficulty::MEDIUM) AImove_Medium();
        else AImove_Hard();

        CheckWinner();
        if (currentState == GameState::PLAYING) currentPlayer = Cell::X;
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

    void run() {
        InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Tic-Tac-Toe - C++ Arcade");
        currentScreen = Screen::MENU;

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            if (currentScreen == Screen::MENU) {
                DrawText("TIC TAC TOE", 180, 100, 40, DARKGRAY);
                if (DrawButton("HUMANO VS HUMANO", {150, 250, 300, 50})) {
                    selectedOpponent = Opponent::HUMAN;
                    InitGame();
                    currentScreen = Screen::GAME;
                }
                if (DrawButton("HUMANO VS IA", {150, 320, 300, 50})) {
                    selectedOpponent = Opponent::AI;
                    currentScreen = Screen::DIFFICULTY_SELECT;
                }

            } else if (currentScreen == Screen::DIFFICULTY_SELECT) {
                DrawText("NIVEL DA IA", 200, 100, 30, DARKGRAY);
                if (DrawButton("FACIL (Random)", {200, 200, 200, 40})) { aiDifficulty = Difficulty::EASY; InitGame(); currentScreen = Screen::GAME; }
                if (DrawButton("MEDIO (Hibrido)", {200, 260, 200, 40})) { aiDifficulty = Difficulty::MEDIUM; InitGame(); currentScreen = Screen::GAME; }
                if (DrawButton("DIFICIL (Minimax)", {200, 320, 200, 40})) { aiDifficulty = Difficulty::HARD; InitGame(); currentScreen = Screen::GAME; }
                if (DrawButton("VOLTAR", {200, 380, 200, 40})) { currentScreen = Screen::MENU;}

            } else if (currentScreen == Screen::GAME) {
                if (currentState == GameState::PLAYING) {
                    if (currentPlayer == Cell::X || (currentPlayer == Cell::O && selectedOpponent == Opponent::HUMAN)) {
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            int col = GetMouseX() / CELL_SIZE;
                            int row = GetMouseY() / CELL_SIZE;
                            if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == Cell::EMPTY) {
                                board[row][col] = currentPlayer;
                                CheckWinner();
                                if (currentState == GameState::PLAYING) {
                                    currentPlayer = (currentPlayer == Cell::X) ? Cell::O : Cell::X;
                                    if (selectedOpponent == Opponent::AI && currentPlayer == Cell::O) {
                                        ExecuteAI();
                                    }
                                }
                            }
                        }
                    }
                } else {
                    if (IsKeyPressed(KEY_R)) InitGame();
                    if (IsKeyPressed(KEY_M)) currentScreen = Screen::MENU;
                }

                for (int i = 1; i < 3; i++) {
                    DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_SIZE, LIGHTGRAY);
                    DrawLine(0, i * CELL_SIZE, SCREEN_SIZE, i * CELL_SIZE, LIGHTGRAY);
                }
                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 3; c++) {
                        int cx = c * CELL_SIZE + CELL_SIZE / 2;
                        int cy = r * CELL_SIZE + CELL_SIZE / 2;
                        if (board[r][c] == Cell::X) {
                            DrawLineEx({(float)cx - 40, (float)cy - 40}, {(float)cx + 40, (float)cy + 40}, 10, RED);
                            DrawLineEx({(float)cx + 40, (float)cy - 40}, {(float)cx - 40, (float)cy + 40}, 10, RED);
                        } else if (board[r][c] == Cell::O) DrawCircleLines(cx, cy, 45, BLUE);
                    }
                }

                if (currentState != GameState::PLAYING) {
                    DrawRectangle(0, 250, 600, 100, Fade(BLACK, 0.8f));
                    const char* t = (currentState == GameState::WIN_X) ? "X VENCEU!" : (currentState == GameState::WIN_O) ? "O VENCEU!" : "EMPATE!";
                    DrawText(t, 300 - MeasureText(t, 30)/2, 285, 30, WHITE);
                    DrawText("[R] Reiniciar  [M] Menu", 300 - MeasureText("[R] Reiniciar  [M] Menu", 20)/2, 320, 20, LIGHTGRAY);
                }
            }

            EndDrawing();
        }
        CloseWindow();
    }
}