#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	//Where is player
	Vector2f m_Position;
	//Sprite
	Sprite m_Sprite;
	//texture
	//Changes here soon!!
	Texture m_Texture;
	//Screen resolution
	Vector2f m_Resolution;
	//Current size of arena
	IntRect m_Arena;
	//Tile size
	int m_Tilesize;
	//Directions
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	//Health player has 
	int m_Health;

	//Max health player can have
	int m_MaxHealth;

	//When was player last hit
	Time m_LastHit;
	//Speed in pixels per second
	float m_Speed;

	//Public Functions Next
public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	//Called at the end of every game
	void resetPlayerStatus();

	//Handle the player getting hit by a zombie
	bool hit(Time timeHit);

	//How long ago was the player last hit
	Time getLastHitTime();

	//Where is the player
	FloatRect getPosition();

	//Where is the center of the player
	Vector2f getCenter();
	
	//What angle is the playe facing
	float getRotation();

	//Send a copy of the sprite to the main function;
	Sprite getSprite();

	//Move functins
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	//Stop player from moving
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	//Called every frame
	void update(float elaspedTime, Vector2i mousePosition);

	//Give the player a speed boost
	void upgradeSpeed();

	//Give player some health
	void upgradeHealth();
	//Increase max Health
	void increaseHealthLevel(int amount);
	//How much health ahs the player currently got
	int getHealth();

};

