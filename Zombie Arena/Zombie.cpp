#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

void Zombie::spawn(float startX, float startY, int type, int seed) {

	switch (type) {

	case 0:
		//Bloater
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));

		m_Speed = BLOATER_SPEED;
		m_Health = BLOATER_HEALTH;
		break;
	case 1:
		//Chaser
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		m_Speed = CHASER_SPEED;
		m_Health = CHASER_HEALTH;
		break;
	case 2:
		//Crawler
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = CRAWLER_SPEED;
		m_Health = CRAWLER_HEALTH;
		break;
	}

	//Modify the speed to make the zombie unique
	
	srand((int)time(0) * seed);
	//Somewhere btween 80 and 100
	float modifier = (rand() % MAX_VARIANCE) + OFFSET;
	//Express as a fraction of 1
	modifier /= 100; //Now equals between .7 to 1
	m_Speed *= modifier;

	//Initialize its location
	m_Position.x = startX;
	m_Position.y = startY;
	//Set its origin to its center
	m_Sprite.setOrigin(25, 25);
	//Set the position
	m_Sprite.setPosition(m_Position);

}

bool Zombie::hit() {

	m_Health--;
	if (m_Health < 0) {
		//dead
		m_Alive = false;

		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	//Injured but not dead yet
	return false;
}

bool Zombie::isAlive() {
	return m_Alive;
}

FloatRect Zombie::getPosition() {
	return m_Sprite.getGlobalBounds();
}

Sprite Zombie::getSprite() {
	return m_Sprite;
}

void Zombie::update(float elaspedTime, Vector2f playerLocation) {
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	//Update the zombie position variables
	if (playerX > m_Position.x) {
		m_Position.x = m_Position.x + m_Speed * elaspedTime;
	}
	
	if (playerY > m_Position.y) {
		m_Position.y = m_Position.y + m_Speed * elaspedTime;
	}

	if (playerX < m_Position.x) {
		m_Position.x = m_Position.x - m_Speed * elaspedTime;
	}

	if (playerY < m_Position.y) {
		m_Position.y = m_Position.y - m_Speed * elaspedTime;
	}

	//Move the sprite
	m_Sprite.setPosition(m_Position);
	//Face the sprite in the correct direction
	float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x) * 180) / 3.141;

	m_Sprite.setRotation(angle);
}