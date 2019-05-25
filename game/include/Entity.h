#ifndef ENTITY_H
#define ENTITY_H

#include "../../stage3/stage3.h"
#include <glm/glm.hpp>

/*
An entity is a 3d object that moves around the world.
*/
class Entity
{
public:
	const glm::vec3& getPos();
	void setPos(glm::vec3 newPos);

	const glm::vec3& getVelocity();
	void setVelocity(glm::vec3 newVelocity);

	const float& getRadius();
	void setRadius(float newRadius);

	virtual void update(float dt) = 0;
private:
	glm::vec3 pos;
	glm::vec3 velocity;

	//this will be used for collision
	float radius;
};

#endif 
