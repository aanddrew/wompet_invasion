#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "../../game/include/Player.h"
#include "../../stage3/stage3.h"

#include <glm/glm.hpp>

class PlayerController
{
public:
	PlayerController(Player* pIn);

	//player controls game via these inputs
	void keyInput(int key , bool pressed);
	void mouseInput(int dx, int dy);

	void update(float dt);

	const float RUN_SPEED = 24.0f;
	const float WALK_SPEED = 12.0f;
private:
	float moveSpeed;
	float mouseSens;

	//movement flags
	enum 
	{
		RIGHT,
		LEFT,
		FORWARD,
		BACK,
		JUMP,
		WALK,
		NUM_MOVES,
	};
	bool moving[NUM_MOVES];

	//this is the last change in velocity we made the the player.
	//subtract it out every time we update
	glm::vec3 dv;

	float deltaRotateUp;
	float deltaRotateRight;

	//these are the controllees
	Player* player;
	Camera* camera;
};

#endif