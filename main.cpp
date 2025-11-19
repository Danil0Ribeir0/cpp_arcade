#include <iostream>
#include "src/tictactoe/TicTacToe.h"

int main() {
    int choice = 0;

    std::cout << "========================\n";
    std::cout << "      CPP ARCADE        \n";
    std::cout << "========================\n";
    std::cout << "1. Tic-Tac-Toe\n";
    std::cout << "2. Sair\n";
    std::cout << "Escolha o jogo: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            TicTacToe::run(); // Chama o jogo usando o namespace
            break;
        case 2:
            std::cout << "Saindo...\n";
            break;
        default:
            std::cout << "Opcao invalida!\n";
    }

    return 0;
}