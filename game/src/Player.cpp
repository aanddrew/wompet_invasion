#include "../include/Player.h"

Player::Player()
:
camera()
{
	setRadius(3);
	cash = 0;

	setPos(glm::vec3(0.0f, 0.0f, 5.0f));
	setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Player::update(float dt)
{
	setPos(getPos() + getVelocity()*dt);
	//camera follows the player
	camera.setPos(getPos());
}

Camera& Player::getCamera()
{
	return camera;
}