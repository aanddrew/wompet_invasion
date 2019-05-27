#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "../../stage3/stage3.h"

class Player : public Entity
{
public:
	Player();

	void update(float dt);

	Camera& getCamera();
private:
	unsigned int cash;

	Camera camera;
};

#endif 