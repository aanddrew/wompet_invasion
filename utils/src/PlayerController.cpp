#include "../include/PlayerController.h"

PlayerController::PlayerController(Player* pIn)
:
dv(0.0f, 0.0f, 0.0f)
{
	moveSpeed = 24.0f;
	mouseSens = 1.0f;

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
		case SDLK_LSHIFT:
			moving[WALK] = pressed;
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

	glm::vec3 alignedForward(player->getCamera().getForward()[0], 0.0f, player->getCamera().getForward()[2]);
	alignedForward = glm::normalize(alignedForward);

	if (moving[WALK])
		moveSpeed = WALK_SPEED;
	else
		moveSpeed = RUN_SPEED;

	if (moving[FORWARD])
	{
		dv[0] += alignedForward[0] * moveSpeed; 
		dv[1] += alignedForward[1] * moveSpeed;
		dv[2] += alignedForward[2] * moveSpeed;
	}
	if (moving[BACK])
	{
		dv[0] += alignedForward[0] * -1* moveSpeed; 
		dv[1] += alignedForward[1] * -1* moveSpeed;
		dv[2] += alignedForward[2] * -1* moveSpeed;
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
	camera->rotateUp(deltaRotateUp*0.003f);
	camera->rotateRight(deltaRotateRight*0.003f);

	//clear our mouse rotations
	deltaRotateUp = 0.0f;
	deltaRotateRight = 0.0f;

	if (moving[JUMP])
	{
		moving[JUMP] = false;
		player->setVelocity(player->getVelocity() + glm::vec3(0.0f, 15.0f, 0.0f));
	}
}