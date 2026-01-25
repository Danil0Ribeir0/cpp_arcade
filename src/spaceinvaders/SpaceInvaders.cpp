#include "SpaceInvaders.h"

namespace SpaceInvaders {

    void Game::Init() {
        score = 0;
        lives = 3;
        wave = 1;
        running = true;
        paused = false;
        
        player.rect = { (float)SCREEN_WIDTH / 2 - 20, (float)SCREEN_HEIGHT - 50, 40, 20 };
        player.active = true;
        player.color = GREEN;

        aliens.clear();
        bullets.clear();
        bunkers.clear();

        int rows = 5;
        int cols = 11;
        float startX = 50;
        float startY = HEADER_HEIGHT + 50;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                Entity alien;
                alien.rect = { startX + col * 50, startY + row * 40, 30, 30 };
                alien.active = true;
                
                if (row == 0) alien.color = PURPLE;
                else if (row < 3) alien.color = BLUE;
                else alien.color = MAROON;

                aliens.push_back(alien);
            }
        }

        for (int i = 0; i < 4; i++) {
            Entity bunker;
            bunker.rect = { 100.0f + i * 180, (float)SCREEN_HEIGHT - 120, 60, 40 };
            bunker.active = true;
            bunker.color = GRAY;
            bunkers.push_back(bunker);
        }

        alienDirection = 1;
        moveInterval = 0.5f;
        lastMoveTime = GetTime();
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
        int titleW = MeasureText("SPACE INVADERS", 40);
        DrawText("SPACE INVADERS", SCREEN_WIDTH/2 - titleW/2, 150, 40, LIME);

        if (DrawButton("INICIAR", {(float)SCREEN_WIDTH/2 - 100, 300, 200, 50})) {
            Init();
            currentScreen = Screen::GAME;
        }

        if (DrawButton("SAIR", {(float)SCREEN_WIDTH/2 - 100, 380, 200, 50})) {
            exitGame = true;
        }

        const char* inst = "Setas: Mover | Espaco: Atirar";
        DrawText(inst, SCREEN_WIDTH/2 - MeasureText(inst, 20)/2, SCREEN_HEIGHT - 50, 20, LIGHTGRAY);
    }

    void Game::FireAlienBullet() {
        if (aliens.empty()) return;
        
        int shooterIndex = GetRandomValue(0, aliens.size() - 1);
        Entity& shooter = aliens[shooterIndex];
        
        if (shooter.active) {
            Bullet b;
            b.rect = { shooter.rect.x + shooter.rect.width/2 - 2, shooter.rect.y + shooter.rect.height, 4, 10 };
            b.speed = 5.0f; 
            b.active = true;
            b.playerBullet = false;
            bullets.push_back(b);
        }
    }

    void Game::Update() {
        if (currentScreen == Screen::MENU) return;
        
        if (lives <= 0 || aliens.empty()) {
            running = false;
            if (IsKeyPressed(KEY_R)) Init();
            if (IsKeyPressed(KEY_M)) currentScreen = Screen::MENU;
            return;
        }

        if (IsKeyPressed(KEY_P)) paused = !paused;
        if (paused) return;

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.rect.x -= 5.0f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.rect.x += 5.0f;

        if (player.rect.x < 0) player.rect.x = 0;
        if (player.rect.x + player.rect.width > SCREEN_WIDTH) player.rect.x = SCREEN_WIDTH - player.rect.width;

        if (IsKeyPressed(KEY_SPACE)) {
            bool bulletActive = false;
            for (const auto& b : bullets) if (b.playerBullet && b.active) bulletActive = true;
            
            if (!bulletActive) {
                Bullet b;
                b.rect = { player.rect.x + player.rect.width/2 - 2, player.rect.y, 4, 10 };
                b.speed = -7.0f; 
                b.active = true;
                b.playerBullet = true;
                bullets.push_back(b);
            }
        }

        if (GetRandomValue(0, 100) < 2) FireAlienBullet();

        for (auto& b : bullets) {
            if (!b.active) continue;
            b.rect.y += b.speed;
            
            if (b.rect.y < HEADER_HEIGHT || b.rect.y > SCREEN_HEIGHT) {
                b.active = false;
                continue;
            }

            if (b.playerBullet) {
                for (auto& alien : aliens) {
                    if (alien.active && CheckCollisionRecs(b.rect, alien.rect)) {
                        alien.active = false;
                        b.active = false;
                        score += 20;
                        
                        moveInterval *= 0.98f; 
                        break;
                    }
                }
            } else {
                if (CheckCollisionRecs(b.rect, player.rect)) {
                    lives--;
                    b.active = false;
                    player.rect.x = SCREEN_WIDTH / 2 - 20; 
                }
            }

            for (auto& bunker : bunkers) {
                if (bunker.active && CheckCollisionRecs(b.rect, bunker.rect)) {
                    b.active = false;
                    bunker.rect.width -= 10; 
                    bunker.rect.x += 5;
                    if (bunker.rect.width <= 10) bunker.active = false;
                }
            }
        }

        std::erase_if(bullets, [](const Bullet& b) { return !b.active; });
        std::erase_if(aliens, [](const Entity& a) { return !a.active; });

        if (GetTime() - lastMoveTime > moveInterval) {
            lastMoveTime = GetTime();
            bool moveDown = false;

            for (const auto& alien : aliens) {
                if ((alienDirection == 1 && alien.rect.x + alien.rect.width > SCREEN_WIDTH - 20) ||
                    (alienDirection == -1 && alien.rect.x < 20)) {
                    moveDown = true;
                    break;
                }
            }

            if (moveDown) {
                alienDirection *= -1;
                for (auto& alien : aliens) alien.rect.y += 20;
            } else {
                for (auto& alien : aliens) alien.rect.x += 10 * alienDirection;
            }
            
            for(const auto& alien : aliens) {
                if (alien.rect.y + alien.rect.height >= player.rect.y) lives = 0; 
            }
        }
    }

    void Game::Draw() {
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen == Screen::MENU) {
            DrawMenu();
        } else {
            DrawRectangle(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, DARKGRAY);
            DrawText(TextFormat("SCORE: %04i", score), 20, 20, 20, GREEN);
            DrawText(TextFormat("LIVES: %i", lives), SCREEN_WIDTH - 100, 20, 20, GREEN);

            if (!running) {
                const char* msg = (lives > 0) ? "YOU WON!" : "GAME OVER";
                DrawText(msg, SCREEN_WIDTH/2 - MeasureText(msg, 40)/2, SCREEN_HEIGHT/2, 40, (lives>0)?GREEN:RED);
                DrawText("[R] Reiniciar  [M] Menu", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 60, 20, WHITE);
            } else {
                if (paused) DrawText("PAUSED", SCREEN_WIDTH/2 - 40, SCREEN_HEIGHT/2, 30, WHITE);

                DrawRectangleRec(player.rect, player.color);
                
                // Desenha canhãozinho
                DrawRectangle(player.rect.x + 15, player.rect.y - 5, 10, 5, GREEN);

                for (const auto& alien : aliens) {
                    DrawRectangleRec(alien.rect, alien.color);
                    // Olhos dos aliens
                    DrawRectangle(alien.rect.x + 5, alien.rect.y + 10, 5, 5, BLACK);
                    DrawRectangle(alien.rect.x + 20, alien.rect.y + 10, 5, 5, BLACK);
                }

                for (const auto& bunker : bunkers) {
                    if (bunker.active) DrawRectangleRec(bunker.rect, bunker.color);
                }

                for (const auto& b : bullets) {
                    DrawRectangleRec(b.rect, b.playerBullet ? YELLOW : RED);
                }
            }
        }
        EndDrawing();
    }

    void Game::RunLoop() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders - C++ Arcade");
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