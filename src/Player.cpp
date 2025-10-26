#include "Player.h"

#include <cmath>

using namespace Cursed3DEngine;

Player::Player(float startingX, float startingY, float startingRotationAngle) 
    : position(startingX, startingY), rotationAngle(startingRotationAngle) {}

Player::~Player() {}

const Vector2F& Player::getPosition() const
{
    return position;
}

void Player::setPosition(const Vector2F& position)
{
    this->position = position;
}

float Player::getRotationAngle() const
{
    return rotationAngle;
}

void Player::setRotationAngle(float rotationAngle)
{
    this->rotationAngle = rotationAngle;
}

float Player::getSpeed() const
{
    return speed;
}

void Player::setSpeed(float speed)
{
    this->speed = speed;
}

void Player::applyMovement(PlayerMovement movement, float elapsedTime, const Map &map)
{
    float moveX = 0.0f;
    float moveY = 0.0f;
    switch (movement) {
        case MOVE_FORWARD:
            moveX = sinf(rotationAngle) * speed * elapsedTime;
            moveY = cosf(rotationAngle) * speed * elapsedTime;
			position += Vector2F(moveX, moveY);
            if (map[(int)position.x* map.getWidth() + (int)position.y] == WALL_SYMBOL) {
				position -= Vector2F(moveX, moveY);
			}	
            break;

        case MOVE_BACKWARD:
            moveX = sinf(rotationAngle) * speed * elapsedTime;
            moveY = cosf(rotationAngle) * speed * elapsedTime;
			position -= Vector2F(moveX, moveY);
            if (map[(int)position.x* map.getWidth() + (int)position.y] == WALL_SYMBOL) {
				position += Vector2F(moveX, moveY);
			}	
            break;

        case ROTATE_CR:
            rotationAngle += (speed * 0.75f) * elapsedTime;
            break;

        case ROTATE_CCR:
            rotationAngle -= (speed * 0.75f) * elapsedTime;
            break;
    }
}