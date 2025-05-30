#include <iostream>
#include <array>
#include <ctime>

class TicTacToe {
protected:
    std::array<char, 9> spaces;
    char player;
    char player2;
    char computer;
    bool running;

public:
    //Constructor
    TicTacToe();

    // Methods
    void drawBoard(char mode);
    void playerMove(char currentPlayer);
    void computerMove();
    bool checkWinner(char currentPlayer);
    bool checkTie();
    void startGame(char mode);
    void drawTitle();
};