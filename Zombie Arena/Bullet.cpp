#include "Bullet.h"

//Constructor
Bullet::Bullet() {
	m_BulletShape.setSize(sf::Vector2f(2, 2));
}

void Bullet::stop() {
	m_InFlight = false;
}

bool Bullet::isInFlight() {
	return m_InFlight;
}

void Bullet::shoot(float startX, float startY, float xTarget, float yTarget) {

	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;
	//Calculate the gradietn of the flight path
	float gradient = (startX - xTarget) / (startY - yTarget);


	//Any gradient less than 1 needs to be negative
	if (gradient < 0) {
		gradient *= -1;
	}
	//Calculate ratio between x and y
	float ratioXY = m_BulletSpeed / (1 + gradient);
	//set the "speed" horizontally and vertically
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	//Point the bullet in the right direction
	if (xTarget < startX) {
		m_BulletDistanceX *= -1;

	}
	if (yTarget < startY) {
		m_BulletDistanceY *= -1;
	}

	//Set max range of 1000 pixels
	float range = 1000;

	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	//Pposition bullet ready to be drawn

	m_BulletShape.setPosition(m_Position);
}

FloatRect Bullet::getPosition() {
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape() {
	return m_BulletShape;
}

void Bullet::update(float elaspedTime) {
	//Update the bullet position variables
	m_Position.x += m_BulletDistanceX * elaspedTime;
	m_Position.y += m_BulletDistanceY * elaspedTime;
	//Move the bullet
	m_BulletShape.setPosition(m_Position);
	//Has the bullet gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX || m_Position.y < m_MinY || m_Position.y > m_MaxY) {
		m_InFlight = false;
	}

}
