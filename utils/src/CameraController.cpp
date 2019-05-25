#include "../include/CameraController.h"

#include <SDL2/SDL.h>
#include <iostream>

CameraController::CameraController(Camera* camIn)
{
	camera = camIn;
	for(int i = 0; i < NUM_MOVES; i++)
	{
		moving[i] = 0;
	}
}

void CameraController::keyInput(int key , bool pressed)
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
		case SDLK_SPACE:
			moving[UP] = pressed;
		break;
		case SDLK_LSHIFT:
			moving[DOWN] = pressed;
		break;
	}
}

void CameraController::mouseInput(int dx, int dy)
{
	// std::cout << "mouse movement: " << dx << ", " << dy << std::endl;
	camera->rotateUp(-1*dy * 0.01);
	camera->rotateRight(-1*dx * 0.01);
}

void CameraController::update(float dt)
{
	glm::vec3 forward = camera->getForward();
	glm::vec3 right = camera->getRight();

	float moveSpeed = 20 * dt;

	if (moving[FORWARD])
	{
		camera->translate(glm::vec3(forward[0]*moveSpeed, forward[1]*moveSpeed, forward[2] * moveSpeed));
	}
	if (moving[BACK])
	{
		camera->translate(glm::vec3(-1*forward[0]*moveSpeed, -1*forward[1]*moveSpeed, -1*forward[2] * moveSpeed));
	}
	if (moving[LEFT])
	{
		camera->translate(glm::vec3(-1*right[0]*moveSpeed, -1*right[1]*moveSpeed, -1*right[2]*moveSpeed));
	}
	if (moving[RIGHT])
	{
		camera->translate(glm::vec3(right[0]*moveSpeed, right[1]*moveSpeed, right[2]*moveSpeed));
	}
	if (moving[UP])
	{
		camera->translate(glm::vec3(0, moveSpeed, 0));
	}
	if (moving[DOWN])
	{
		camera->translate(glm::vec3(0, -moveSpeed, 0));
	} 
}