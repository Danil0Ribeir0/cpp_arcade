#include <iostream>
#include "src/tictactoe/TicTacToe.h"
#include "src/pong/Pong.h"

int main() {
    int choice = 0;

    std::cout << "========================\n";
    std::cout << "      CPP ARCADE        \n";
    std::cout << "========================\n";
    std::cout << "1. Tic-Tac-Toe\n";
    std::cout << "2. Pong\n";
    std::cout << "3. Sair\n";
    std::cout << "Escolha o jogo: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            TicTacToe::run();
            break;
        case 2:
            Pong::run();
            break;
        case 3:
            std::cout << "Saindo...\n";
            break;
        default:
            std::cout << "Opcao invalida!\n";
    }

    return 0;
}