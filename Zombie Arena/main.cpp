#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
using namespace sf;

int main() {

	srand(static_cast<unsigned>(time(0)));


	//Instance of TextureHolder
	TextureHolder holder;

	//The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	//Start with the GAME_OVER state
	State state = State::GAME_OVER;
	//Get screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	//Create a SFML view for main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//Clock for timing
	Clock clock;

	//Time for how long PLAYING game state is active
	Time gameTimeTotal;

	//Mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	//Mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	//Instance of player class
	Player player;

	//Boundaries of our arena
	IntRect arena;

	//Create background
	VertexArray background;
	//Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	//Prepare the horde
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//100 Bullets 
	Bullet bullets[200];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	//When was last bullet fired
	Time lastPressed;

	//Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	//Pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	int score = 0;
	int hiScore = 0;
	//For the home/game over screen

	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	//Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, 1920, 1080));
	//Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
	//Load the font
	Font font;
	font.loadFromFile("fonts/minecraft_font.ttf");
	//Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter to Continue");
	//Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to Play");
	//Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream << "1 - Increased rate of fire" << std::endl
		<< "2 - Increased clip size (next reload)" << std::endl
		<< "3 - Increased max health" << std::endl
		<< "4 - Increased run speed" << std::endl
		<< "5 - More and better health pickups" << std::endl
		<< "6 - More and better ammo pickups" << std::endl;
	levelUpText.setString(levelUpStream.str());
	//Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(35);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);
	//Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(35);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	//Load the high score from a text file
	std::ifstream inputFile("gamedata / scores.txt");
	if (inputFile.is_open()) {

		//Reads the data
		inputFile >> hiScore;
		inputFile.close();
	}

	//Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(35);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());
	//Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");
	//Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(35);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");
	//Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	//When  did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	//How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	//Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);	
	//Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);
	//Prepare the shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);
	//Reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);
	//Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);
	//Powerup Sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");	
	Sound powerup;
	powerup.setBuffer(powerupBuffer);
	//Pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");	
	Sound pickup;
	pickup.setBuffer(pickupBuffer);



	//main game loop window
	while (window.isOpen()) {
		/*Handle input*/

		//Handle events by polling
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {

				//Pause a game whikle playing
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				}
				//Restart whikle paused
				else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					//Reset clock so there isnt a frame jump
					clock.restart();
				}
				//Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state = State::LEVELING_UP;

					wave = 0;
					score = 0;

					//Prepare the gun and ammo for next game
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;	
					fireRate = 1;

					//reset player stats
					player.resetPlayerStatus();
				}
				if (state == State:: PLAYING){
					//Reloading
					if (event.key.code == Keyboard::R) {
						if (bulletsSpare >= clipSize) {
							//Lots of bullets. Reload
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0) {
							//Only few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else {
							reloadFailed.play();
						}

					}
				}
			}
		}//End event polling

		//Handle player quitting 
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		//handle WASD while playing
		if (state == State::PLAYING) {
			//Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else {
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else {
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else {
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else {
				player.stopRight();
			}

			//Fire a bullet
			if (Mouse::isButtonPressed(sf::Mouse::Left)) {

				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0) {
					//Pass the center of the player and center of cross hair to the shoot function
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99) {
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					shoot.play();
					bulletsInClip--;

				}
			}//End fire a bullet

		}//End WASD while playing

		

		//Handle the LEVELING up state
		if (state == State::LEVELING_UP) {
			//Leveling up the player
			if (event.key.code == Keyboard::Num1) {
				fireRate++;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2) {
				clipSize += clipSize;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3) {
				//Increase Health
				player.upgradeHealth();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4) {
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5) {
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6) {
				ammoPickup.upgrade();	
				state = State::PLAYING;
			}

			if (state == State::PLAYING) {
				wave++;
				
				
				//Prepare level
				arena.width = 1500;
				arena.height = 1500;
				arena.left = 0;
				arena.top = 0;
				//Pass vertex array by reference to the createBackground function
				int tileSize = createBackground(background, arena);
				//We will modify this line of code later
				//int tileSize = 50;
				//Spawn player in the middle of arena

				player.resetPlayerStatus();

				player.spawn(arena, resolution, tileSize);

				//Configure pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);



				//Create a horde
				numZombies = std::min(500, 5 * wave);
				//Delete the previously alllocated memory( if it exists)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				powerup.play();

				//reset clock 
				clock.restart();
			}
		}//end Leveling UP

		/*Update the Frame*/
		if (state == State::PLAYING) {
			//Update the delta time
			Time dt = clock.restart();

			//Update game time total
			gameTimeTotal += dt;

			//Make a faction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();
			//Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();
			//Convert mouse position to world
			//based coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
			//Set the crosshair to the mouse world positon
			spriteCrosshair.setPosition(mouseWorldPosition);
			//Update the player
			player.update(dtAsSeconds, Mouse::getPosition());
			//Make note of new position
			Vector2f playerPosition(player.getCenter());

			//Make view center around player
			mainView.setCenter(player.getCenter());

			//Loop through each Zombie and update them
			for (int i = 0; i < numZombies; i++) {

				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			//Update any bullets that are in-flight
			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					bullets[i].update(dtAsSeconds);
				}
			}

			//Update pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			//Collision updates
			//Have any Zombies been shot?
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numZombies; j++) {
					if (bullets[i].isInFlight() && zombies[j].isAlive()) {
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
							//Stop the bullet
							bullets[i].stop();
							//Register the hit 
							if (zombies[j].hit()) {
								score += 10;
								if (score >= hiScore) {
									hiScore = score;

								}
								numZombiesAlive--;
								//When all zombies are dead
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}
							splat.play();
						}
					}
				}
			} //End zombie being shot

			//Have the zombies touched the player
			for (int i = 0; i < numZombies; i++) {
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()) {
					if (player.hit(gameTimeTotal)) {
						hit.play();
					}
					if (player.getHealth() <= 0) {
						state = State::GAME_OVER;
						std::ofstream outputFile("gamedata/scores.txt");
						//writes the data
						outputFile << hiScore;
						outputFile.close();

						score = 0;
					}
				}
			}//End player touched

			//Has player touched a pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				player.increaseHealthLevel(healthPickup.gotIt());
				pickup.play();

			}
			//Has the playerr touched ammo pickup
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				bulletsSpare += ammoPickup.gotIt();
				reload.play();
			}

			//Size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			//Increment the number of frames since the previous update
			framesSinceLastHUDUpdate++;
			//re calculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {
				//update game HUD
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				//Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				//Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				//Update the hi score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());
				//Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());
				//Update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}//End HUD update

		}//end updating scene

		/*Draw the scene*/
		if (state == State::PLAYING) {
			window.clear();
			//set the mainView to be displayed in the window and draw everyting related to it
			window.setView(mainView);

			//Draw background
			window.draw(background, &textureBackground);

			//Draw the zombies
			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			//Draw bullets
			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					window.draw(bullets[i].getShape());
				}
			}

			//Draw player
			window.draw(player.getSprite());

			//Draw pickups
			if (ammoPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}

			if (healthPickup.isSpawned()) {
				window.draw(healthPickup.getSprite());
			}

			//draw the crosshair
			window.draw(spriteCrosshair);

			//Switch to HUD view
			window.setView(hudView);
			//Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);	
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if (state == State::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);

		}
		if (state == State::PAUSED) {
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER) {
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
	}//End game loop

	//Delete allocated memory
	delete[] zombies;
	return 0;
}

