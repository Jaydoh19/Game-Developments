#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


class Zombie
{
private:
	//How fast is each zombie
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//How tough is each zombie
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//Make each zombie vary its speed slighty
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	//Where is the zombie
	Vector2f m_Position;

	//Sprite
	Sprite m_Sprite;

	//How fast can they run/crawl
	float m_Speed;

	//Health
	float m_Health;
	
	//Is it alive?
	bool m_Alive;

public:
	//Handle when a bullet hits a zombie
	bool hit();
	//Find out if the zombie is alive
	bool isAlive();
	//Spawn a new zombie
	void spawn(float startX, float startY, int type, int seed);
	//return a rectangle that is the position in the world
	FloatRect getPosition();
	//Get a opy of the sprite to draw
	Sprite getSprite();
	//Update the zombie each fra,e
	void update(float elapsedTime, Vector2f playerLocation);

};

