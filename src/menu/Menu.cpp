#include "Menu.h"

namespace Menu {

    // Função auxiliar para desenhar botões (reutilizada dos jogos, mas adaptada)
    bool DrawMenuButton(const char* text, Rectangle rect, Color color) {
        bool clicked = false;
        Vector2 mousePoint = GetMousePosition();
        bool hover = CheckCollisionPointRec(mousePoint, rect);

        // Efeito de Hover (fica mais claro)
        Color drawColor = hover ? ColorBrightness(color, 0.2f) : color;

        DrawRectangleRec(rect, drawColor);
        DrawRectangleLinesEx(rect, 2, BLACK);

        // Centraliza texto
        int fontSize = 20;
        int textW = MeasureText(text, fontSize);
        DrawText(text, rect.x + rect.width/2 - textW/2, rect.y + rect.height/2 - fontSize/2, fontSize, WHITE);

        if (hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) clicked = true;
        return clicked;
    }

    GameOption Show() {
        // Configuração da Janela do Menu
        const int screenWidth = 800;
        const int screenHeight = 600;

        InitWindow(screenWidth, screenHeight, "C++ Arcade - Main Menu");
        SetTargetFPS(60);

        GameOption selected = EXIT;
        bool choiceMade = false;

        while (!WindowShouldClose() && !choiceMade) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("C++ ARCADE", screenWidth/2 - MeasureText("CPP ARCADE", 60)/2, 80, 60, DARKBLUE);
            DrawText("Selecione um jogo para comecar", screenWidth/2 - MeasureText("Selecione um jogo para comecar", 20)/2, 150, 20, DARKGRAY);

            float btnW = 220;
            float btnH = 60;
            float spacingX = 40;
            float spacingY = 30;
            float startX = (screenWidth - (btnW * 2 + spacingX)) / 2;
            float startY = 220;

            if (DrawMenuButton("JOGO DA VELHA", {startX, startY, btnW, btnH}, MAROON)) {
                selected = TIC_TAC_TOE; choiceMade = true;
            }
            if (DrawMenuButton("SNAKE", {startX, startY + btnH + spacingY, btnW, btnH}, DARKGREEN)) {
                selected = SNAKE; choiceMade = true;
            }
            if (DrawMenuButton("SPACE INVADERS", {startX, startY + (btnH + spacingY)*2, btnW, btnH}, PURPLE)) {
                selected = SPACE_INVADERS; choiceMade = true;
            }

            if (DrawMenuButton("PONG", {startX + btnW + spacingX, startY, btnW, btnH}, DARKGRAY)) {
                selected = PONG; choiceMade = true;
            }
            if (DrawMenuButton("BREAKOUT", {startX + btnW + spacingX, startY + btnH + spacingY, btnW, btnH}, ORANGE)) {
                selected = BREAKOUT; choiceMade = true;
            }
            if (DrawMenuButton("TETRIS", {startX + btnW + spacingX, startY + (btnH + spacingY)*2, btnW, btnH}, BLUE)) {
                selected = TETRIS; choiceMade = true;
            }

            if (DrawMenuButton("SAIR DO ARCADE", {(float)screenWidth/2 - 100, 520, 200, 40}, BLACK)) {
                selected = EXIT; choiceMade = true;
            }

            DrawText("v1.0 - Feito com Raylib", 10, screenHeight - 20, 10, GRAY);

            EndDrawing();
        }

        CloseWindow();
        return selected;
    }
}