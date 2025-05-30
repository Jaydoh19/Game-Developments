#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <cstdlib>

int main() {
	//Create a video mode object
	VideoMode vm(1920, 1080);
	//Create and opena window 
	RenderWindow window(vm, "Pong", Style::Fullscreen);
	int score = 0;
	int lives = 3;

	//Create a bat at the bottom center of the screen
	Bat bat(1920 / 2, 1080 - 20);
	
	Ball ball(1920 / 2, 0);
	
	//Create a text object called HUD
	Text hud;
	Font font;
	font.loadFromFile("fonts/game_over.ttf");
	//set the font
	hud.setFont(font);
	//Make it nice and big
	hud.setCharacterSize(75);
	//Color
	hud.setFillColor(Color::White);
	hud.setPosition(20, 20);

	//Clock for timing
	Clock clock;

	while (window.isOpen()) {
		/*
		Handle player input
		*/
		Event event;
		while (window.pollEvent(event)){
			
			if (event.type == Event::Closed)
				//quit the game when window is closed
				window.close();
		}

		//Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		
		//handle the pressing and releasing of the arrow keys
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			bat.moveLeft();
		}
		else {
			bat.stopLeft();
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			bat.moveRight();
		}
		else {
			bat.stopRight();
		}




		/*
		Update the bat, the ball and the HUD
		*/
		
		//Update delta time
		Time dt = clock.restart();
		bat.update(dt);
		ball.update(dt);

		//Update HUD text
		std::stringstream ss;
		ss << "Score:" << score << " Lives:" << lives;
		hud.setString(ss.str());

		//Handle the ball hitting the bottom
		if (ball.getPostion().top > window.getSize().y) {
			//reverse the ball direction
			ball.reboundBottom();
			//remove a life
			lives--;
			//check for zero lives
			if (lives < 1) {
				//reset score
				score = 0;
				//reset lives
				lives = 3;
			}
		}

		//Handle the ball hitting the top
		if (ball.getPostion().top < 0) {

			ball.reboundBatOrTop();
			//add a point to score
			score++;

		}

		//Handle the ball hitting sides
		if (ball.getPostion().left < 0 || ball.getPostion().left + ball.getPostion().width > window.getSize().x) {

			ball.reboundSides();
		}

		//Has the ball hit the bat?
		if (ball.getPostion().intersects(bat.getPosition())) {

			//Hit detected so reverse teh ball and score
			ball.reboundBatOrTop();
		}




		/*
		Draw the bat, the ball and HUD
		*/

		window.clear();
		window.draw(hud);
		window.draw(bat.getShape());
		window.draw(ball.getShape());
		window.display();


	}

	return 0;
}