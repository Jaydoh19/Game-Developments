#include "TicTacToe.h"


TicTacToe::TicTacToe() : player('X'), player2('O'), computer('O'), running(true) {
    spaces.fill(' ');
}

void TicTacToe::drawBoard(char mode) {
    std::cout << "___________________________________" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "T I C -- T A C -- T O E -- G A M E" << std::endl;
    std::cout << "___________________________________" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << '\n';
    if (mode == 'c') {
        std::cout << "PLAYER - [X] COMPUTER - [O]";
    }
    else {
        std::cout << "PLAYER - [X] PLAYER2 - [O]";
    }
    std::cout << '\n';
    std::cout << "     |     |     " << '\n';
    std::cout << "  " << spaces[0] << "  |  " << spaces[1] << "  |   " << spaces[2] << "  " << '\n';
    std::cout << "_____|_____|_____" << '\n';
    std::cout << "     |     |     " << '\n';
    std::cout << "  " << spaces[3] << "  |  " << spaces[4] << "  |   " << spaces[5] << "  " << '\n';
    std::cout << "_____|_____|_____" << '\n';
    std::cout << "     |     |     " << '\n';
    std::cout << "  " << spaces[6] << "  |  " << spaces[7] << "  |   " << spaces[8] << "  " << '\n';
    std::cout << "     |     |     " << '\n';
    std::cout << '\n';
}

void TicTacToe::playerMove(char currentPlayer) {
    int number;
    do {
        std::cout << "Enter a spot to place a marker (1-9): ";
        std::cin >> number;

        if (number < 1 || number > 9) {
            std::cout << '\n';
            std::cerr << "Invalid input. Please enter a number between 1 and 9." << std::endl;
            std::cout << '\n';
            continue;
        }

        number--; // Adjust to 0-based index
        if (spaces[number] == ' ') {
            spaces[number] = currentPlayer;
            break;
        }
        else {
            std::cout << '\n';
            std::cerr << "This spot is already taken. Please choose another." << std::endl;
            std::cout << '\n';
        }
    } while (true);

    system("cls"); // Optional, but platform dependent
}

void TicTacToe::computerMove() {
    int number;
    srand(static_cast<unsigned int>(time(0)));

    while (true) {
        number = rand() % 9;
        if (spaces[number] == ' ') {
            spaces[number] = computer;
            break;
        }
    }
    system("cls"); // Optional, but platform dependent
}

bool TicTacToe::checkWinner(char currentPlayer) {
    std::array<std::array<int, 3>, 8> winConditions = { {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    } };

    for (const auto& condition : winConditions) {
        if (spaces[condition[0]] == currentPlayer &&
            spaces[condition[1]] == currentPlayer &&
            spaces[condition[2]] == currentPlayer) {
            std::cout << (currentPlayer == player ? "YOU WIN!\n" : "YOU LOSE!\n");
            return true;
        }
    }

    return false;
}

bool TicTacToe::checkTie() {
    for (char space : spaces) {
        if (space == ' ') {
            return false;
        }
    }
    std::cout << "IT'S A DRAW!\n";
    return true;
}

void TicTacToe::startGame(char mode) {
    drawBoard(mode);

    while (running) {
        playerMove(player);
        drawBoard(mode);
        if (checkWinner(player) || checkTie()) {
            running = false;
            break;
        }

        if (mode == 'c') {
            computerMove();
        }
        else {
            playerMove(player2);
        }
        drawBoard(mode);
        if (checkWinner(mode == 'c' ? computer : player2) || checkTie()) {
            running = false;
            break;
        }
    }

    std::cout << "Thanks for Playing TicTacToe!!!" << std::endl;
}

void TicTacToe::drawTitle() {
    std::cout << "___________________________________" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "T I C -- T A C -- T O E -- G A M E" << std::endl;
    std::cout << "___________________________________" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Would you like to play against a PLAYER or COMPUTER?" << std::endl;
    std::cout << "Enter 'p' for PLAYER or 'c' for COMPUTER" << std::endl;
}