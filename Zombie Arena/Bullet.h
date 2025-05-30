#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Bullet
{
private:
	//Where is bullet
	Vector2f m_Position;
	//Each bullet looks like
	RectangleShape m_BulletShape;
	//Is this bullet in the air?
	bool m_InFlight = false;
	//How fast is it traveling
	float m_BulletSpeed = 1000;

	//What fraction of 1 pixel does the bullet travel? Horizontal or vertical.
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	//Boundaries so the bullet doesnt fly forever
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

public:
	//Constructor
	Bullet();
	//Stop bullet
	void stop();
	//Returns the value of m_InFlight
	bool isInFlight();
	//Launch a new bullet
	void shoot(float startX, float startY, float xTarget, float yTarget);

	//Tell the callign code where the bullet is in the world
	FloatRect getPosition();
	//Return shape (for drawing)
	RectangleShape getShape();
	//Update bullet each frame
	void update(float elaspedTime);

};

