#include "Player.h"
#include "TextureHolder.h"

Player::Player()
	: m_Speed(START_SPEED),
	m_Health(START_HEALTH),
	m_MaxHealth(START_HEALTH),
	m_Texture(),
	m_Sprite()
{
	//Associate a texture with the sprite
	// !Watcht his space!!
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player.png"));

	//Set the origin of the sprite to the center,
	//for smooth rotation
	m_Sprite.setOrigin(25, 25);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;
	//Copy details of the arena
	//to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	m_Tilesize = tileSize;
	
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

void Player::resetPlayerStatus() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}


bool Player::hit(Time timeHit) {
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200) {
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else
	{
		return false;
	}
} 


Time Player::getLastHitTime() {
	return m_LastHit;
}


FloatRect Player::getPosition() {
	return m_Sprite.getGlobalBounds();
}


Vector2f Player::getCenter() {
	return m_Position;
}


float Player::getRotation() {
	return m_Sprite.getRotation();
}


Sprite Player::getSprite() {
	return m_Sprite;
}


void Player::moveLeft() {
	m_LeftPressed = true;
}
void Player::moveRight() {
	m_RightPressed = true;
}
void Player::moveUp() {
	m_UpPressed = true;
}
void Player::moveDown() {
	m_DownPressed = true;
}


void Player::stopLeft() {
	m_LeftPressed = false;
}
void Player::stopRight() {
	m_RightPressed = false;
}
void Player::stopUp() {
	m_UpPressed = false;
}
void Player::stopDown() {
	m_DownPressed = false;
}


void Player::update(float elaspedTime, Vector2i mousePosition) {
	if (m_UpPressed) {
		m_Position.y -= m_Speed * elaspedTime;
	}
	if (m_DownPressed) {
		m_Position.y += m_Speed * elaspedTime;
	}
	if (m_RightPressed) {
		m_Position.x += m_Speed * elaspedTime;
	}
	if (m_LeftPressed) {
		m_Position.x -= m_Speed * elaspedTime;
	}

	// Keep the player in the arena
	if (m_Position.x > m_Arena.width - m_Tilesize) {
		m_Position.x = m_Arena.width - m_Tilesize;
	}
	if (m_Position.x < m_Arena.left + m_Tilesize) {
		m_Position.x = m_Arena.left + m_Tilesize;
	}
	if (m_Position.y > m_Arena.height - m_Tilesize) {
		m_Position.y = m_Arena.height - m_Tilesize;
	}
	if (m_Position.y < m_Arena.top + m_Tilesize) {
		m_Position.y = m_Arena.top + m_Tilesize;
	}

	// Update sprite position to match the player's position
	m_Sprite.setPosition(m_Position);

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2) * 180) / 3.141;
	m_Sprite.setRotation(angle);
}


void Player::upgradeSpeed() {
	//20% speed upgrade
	m_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth() {
	//20% percent health upgrade
	m_MaxHealth += (START_HEALTH * .2);
}

void Player::increaseHealthLevel(int amount) {
	m_Health += amount;
	//But not beyond the max health
	if (m_Health > m_MaxHealth) {
		m_Health = m_MaxHealth;
	}
}

int Player::getHealth() {
	return m_Health;
}