#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Pickup
{
private:
	//Start value for health pickups
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 6;
	const int START_SECONDS_TO_LIVE = 10;

	//Sprite that represents this pickup
	Sprite m_Sprite;

	//aRENA IT EXISTS IN 
	IntRect m_Arena;
	//How much is this pickup worth
	int m_Value;

	//What type of pickup is it
	//1=health 2=ammo
	int m_Type;
	//Handle spawning and disappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

public:
	Pickup(int type);
	//Prepare a new pickup
	void setArena(IntRect arena);
	void spawn();
	//Check the position of the pickup
	FloatRect  getPosition();
	//Get the sprite for drawing
	Sprite getSprite();
	//Let the pickup update itself each frame
	void update(float elaspedTime);
	//Is this pickup currently spawned?
	bool isSpawned();
	//Get the pickup
	int gotIt();
	//Upgrade the value of each pickup
	void upgrade();

};

