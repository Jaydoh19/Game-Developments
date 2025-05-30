#include "TicTacToeSFML.h"
#include <iostream>
#include <random>

TicTacToeSFML::TicTacToeSFML() : TicTacToe(), window(sf::VideoMode(600, 600), "Tic Tac Toe"), mode(' '), currentPlayer('X') {
    // Load font
    if (!font.loadFromFile("fonts/minecraft_font.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    // Title Text
    titleText.setFont(font);
    titleText.setString("Tic Tac Toe");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(150, 50);

    // Play vs Player Option
    playVsPlayerText.setFont(font);
    playVsPlayerText.setString("1. Play vs Player");
    playVsPlayerText.setCharacterSize(30);
    playVsPlayerText.setFillColor(sf::Color::White);
    playVsPlayerText.setPosition(150, 200);

    // Play vs Computer Option
    playVsComputerText.setFont(font);
    playVsComputerText.setString("2. Play vs Computer");
    playVsComputerText.setCharacterSize(30);
    playVsComputerText.setFillColor(sf::Color::White);
    playVsComputerText.setPosition(150, 300);

    // Exit Option
    exitText.setFont(font);
    exitText.setString("3. Exit");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(150, 400);

    // Initialize grid lines
    for (int i = 0; i < 4; i++) {
        gridLines[i].setFillColor(sf::Color::White);
        gridLines[i].setSize(sf::Vector2f(600, 5));
    }
    // Horizontal lines
    gridLines[0].setPosition(0, 200);
    gridLines[1].setPosition(0, 400);
    // Vertical lines
    gridLines[2].setPosition(200, 0);
    gridLines[2].setRotation(90);
    gridLines[3].setPosition(400, 0);
    gridLines[3].setRotation(90);

    // Initialize board spaces
    for (int i = 0; i < 9; i++) {
        boardSpaces[i].setSize(sf::Vector2f(200, 200));
        boardSpaces[i].setFillColor(sf::Color::Transparent);
        boardSpaces[i].setOutlineThickness(-2);
        boardSpaces[i].setOutlineColor(sf::Color::White);
        boardSpaces[i].setPosition((i % 3) * 200, (i / 3) * 200);
    }
}

void TicTacToeSFML::drawMenu() {
    window.clear(sf::Color::Black);
    window.draw(titleText);
    window.draw(playVsPlayerText);
    window.draw(playVsComputerText);
    window.draw(exitText);
    window.display();
}

void TicTacToeSFML::handleMenuInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            exit(0);
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            int x = event.mouseButton.x;
            int y = event.mouseButton.y;

            if (y >= 200 && y < 250) { // Play vs Player
                mode = 'p';
                return;
            }
            else if (y >= 300 && y < 350) { // Play vs Computer
                mode = 'c';
                return;
            }
            else if (y >= 400 && y < 450) { // Exit
                window.close();
                exit(0);
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Num1) { // Play vs Player
                mode = 'p';
                return;
            }
            else if (event.key.code == sf::Keyboard::Num2) { // Play vs Computer
                mode = 'c';
                return;
            }
            else if (event.key.code == sf::Keyboard::Num3) { // Exit
                window.close();
                exit(0);
            }
        }
    }
}

void TicTacToeSFML::drawBoard() {
    window.clear(sf::Color::Black);

    // Draw grid lines
    for (const auto& line : gridLines) {
        window.draw(line);
    }

    // Draw board spaces
    for (const auto& space : boardSpaces) {
        window.draw(space);
    }

    // Render the game markers (X and O)
    for (int i = 0; i < 9; i++) {
        sf::Text markerText("", font, 100);
        markerText.setPosition((i % 3) * 200 + 50, (i / 3) * 200 + 25);

        if (spaces[i] == 'X') {
            markerText.setString("X");
            markerText.setFillColor(sf::Color::Blue);
            window.draw(markerText);
        }
        else if (spaces[i] == 'O') {
            markerText.setString("O");
            markerText.setFillColor(sf::Color::Red);
            window.draw(markerText);
        }
    }

    window.display();
}

void TicTacToeSFML::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            running = false;
        }

        if (mode == 'c') {
            // Player vs Computer logic
            if (currentPlayer == 'X' && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / 200;
                int y = event.mouseButton.y / 200;
                int index = y * 3 + x;

                if (x >= 0 && x < 3 && y >= 0 && y < 3 && spaces[index] == ' ') {
                    spaces[index] = 'X'; // Player move

                    // Check for win or tie
                    if (checkWinner('X')) {
                        std::cout << "Player X Wins!" << std::endl;
                        running = false;
                    }
                    else if (checkTie()) {
                        std::cout << "It's a draw!" << std::endl;
                        running = false;
                    }
                    else {
                        currentPlayer = 'O'; // Switch to computer's turn
                        computerMove();      // Make computer move
                        if (checkWinner('O')) {
                            std::cout << "Computer Wins!" << std::endl;
                            running = false;
                        }
                        else if (checkTie()) {
                            std::cout << "It's a draw!" << std::endl;
                            running = false;
                        }
                        currentPlayer = 'X'; // Return to player's turn
                    }
                }
            }
        }
        else if (mode == 'p') {
            // Player vs Player logic (unchanged)
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / 200;
                int y = event.mouseButton.y / 200;
                int index = y * 3 + x;

                if (x >= 0 && x < 3 && y >= 0 && y < 3 && spaces[index] == ' ') {
                    spaces[index] = currentPlayer;

                    // Check for win or tie
                    if (checkWinner(currentPlayer)) {
                        std::cout << (currentPlayer == 'X' ? "Player X Wins!" : "Player O Wins!") << std::endl;
                        running = false;
                    }
                    else if (checkTie()) {
                        std::cout << "It's a draw!" << std::endl;
                        running = false;
                    }
                    else {
                        togglePlayer();
                    }
                }
            }
        }
    }
}


void TicTacToeSFML::togglePlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

void TicTacToeSFML::computerMove() {
    static std::mt19937 rng(std::random_device{}()); // Random number generator
    std::uniform_int_distribution<int> dist(0, 8);  // Range: 0 to 8

    int index;
    do {
        index = dist(rng); // Generate a random index
    } while (spaces[index] != ' '); // Repeat until an empty space is found

    spaces[index] = 'O'; // Place computer's marker
    std::cout << "Computer placed O at index: " << index << std::endl;
}


void TicTacToeSFML::run() {
    // Display menu and set mode
    while (mode == ' ') {
        drawMenu();
        handleMenuInput();
    }

    // Main game loop
    while (window.isOpen() && running) {
        handleInput(); // Player input or computer move based on mode
        drawBoard();   // Update and display the game board
    }

    // Game over message
    if (!running) {
        std::cout << "Game Over! Thanks for playing!" << std::endl;
        window.close();
    }
}
