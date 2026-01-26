#include "src/menu/Menu.h"
#include "src/tictactoe/TicTacToe.h"
#include "src/pong/Pong.h"
#include "src/snake/Snake.h"
#include "src/breakout/Breakout.h"
#include "src/spaceinvaders/SpaceInvaders.h"
#include "src/tetris/Tetris.h"

int main() {
    bool exitApp = false;

    while (!exitApp) {
        Menu::GameOption choice = Menu::Show();

        switch (choice) {
            case Menu::TIC_TAC_TOE:
                TicTacToe::run();
                break;
            case Menu::PONG:
                Pong::run();
                break;
            case Menu::SNAKE:
                Snake::run();
                break;
            case Menu::BREAKOUT:
                Breakout::run();
                break;
            case Menu::SPACE_INVADERS:
                SpaceInvaders::run();
                break;
            case Menu::TETRIS:
                Tetris::run();
                break;
            case Menu::EXIT:
                exitApp = true;
                break;
            default:
                exitApp = true;
                break;
        }
    }

    return 0;
}