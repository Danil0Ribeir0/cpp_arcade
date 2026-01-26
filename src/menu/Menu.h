#ifndef CPP_ARCADE_MENU_H
#define CPP_ARCADE_MENU_H

#include "raylib.h"

namespace Menu {

    enum GameOption {
        EXIT = 0,
        TIC_TAC_TOE,
        PONG,
        SNAKE,
        BREAKOUT,
        SPACE_INVADERS,
        TETRIS
    };

    GameOption Show();
}

#endif //CPP_ARCADE_MENU_H