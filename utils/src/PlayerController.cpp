#include "../include/PlayerController.h"

PlayerController::PlayerController(Player* pIn)
:
dv(0.0f, 0.0f, 0.0f)
{
	moveSpeed = 16.0f;
	mouseSens = 3.0f;

	player = pIn;
	camera = &(player->getCamera());

	for(int i = 0; i < NUM_MOVES; i++)
	{
		moving[i] = 0;
	}

}

void PlayerController::keyInput(int key, bool pressed)
{
	switch(key)
	{
		//WASD space ctrl
		case SDLK_w:
			moving[FORWARD] = pressed;
		break;
		case SDLK_a:
			moving[LEFT] = pressed;
		break;
		case SDLK_s:
			moving[BACK] = pressed;
		break;
		case SDLK_d:
			moving[RIGHT] = pressed;
		break;
		//jumping
		case SDLK_SPACE:
			moving[JUMP] = pressed;
		break;
	}
}

void PlayerController::mouseInput(int dx, int dy)
{
	// camera->rotateUp(-1*dy * 0.01);
	// camera->rotateRight(-1*dx * 0.01);

	deltaRotateUp = -1*dy * mouseSens;
	deltaRotateRight = -1*dx * mouseSens;
}

void PlayerController::update(float dt)
{
	//erase our change to velocity from last update
	player->setVelocity(player->getVelocity() - dv);

	//reset our dv
	dv = glm::vec3(0.0f, 0.0f, 0.0f);

	if (moving[FORWARD])
	{
		dv[0] += player->getCamera().getForward()[0] * moveSpeed; 
		dv[1] += player->getCamera().getForward()[1] * moveSpeed;
		dv[2] += player->getCamera().getForward()[2] * moveSpeed;
	}
	if (moving[BACK])
	{
		dv[0] += player->getCamera().getForward()[0] * -1* moveSpeed; 
		dv[1] += player->getCamera().getForward()[1] * -1* moveSpeed;
		dv[2] += player->getCamera().getForward()[2] * -1* moveSpeed;
	}
	if (moving[LEFT])
	{
		dv[0] += player->getCamera().getRight()[0] * -1* moveSpeed; 
		dv[1] += player->getCamera().getRight()[1] * -1* moveSpeed;
		dv[2] += player->getCamera().getRight()[2] * -1* moveSpeed;
	}
	if (moving[RIGHT])
	{
		dv[0] += player->getCamera().getRight()[0] * moveSpeed; 
		dv[1] += player->getCamera().getRight()[1] * moveSpeed;
		dv[2] += player->getCamera().getRight()[2] * moveSpeed;
	}

	player->setVelocity(player->getVelocity() + dv);

	//camera rotations
	camera->rotateUp(deltaRotateUp*dt);
	camera->rotateRight(deltaRotateRight*dt);

	//clear our mouse rotations
	deltaRotateUp = 0.0f;
	deltaRotateRight = 0.0f;
}