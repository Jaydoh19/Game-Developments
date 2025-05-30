#include <SFML/Graphics.hpp>
#include <array>
#include "TicTacToe.h"

class TicTacToeSFML : public TicTacToe {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::Text playVsPlayerText;
    sf::Text playVsComputerText;
    sf::Text exitText;
    char mode;           // Game mode ('p' or 'c')
    char currentPlayer;  // Tracks whose turn it is

    sf::RectangleShape gridLines[4];
    sf::RectangleShape boardSpaces[9];
    sf::CircleShape markers[9];

    void drawMenu();       // Renders the menu
    void handleMenuInput(); // Handles input in the menu
    void drawBoard();      // Renders the board
    void handleInput();    // Handles player input
    void togglePlayer();   // Switches between X and O
    void update();         // Updates game state
    void computerMove();
public:
    TicTacToeSFML();
    void run(); // Main game loop
};
