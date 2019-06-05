#ifndef ENEMY_H
#define ENEMY_H

#include <string>

#include "Entity.h"
#include "../../stage3/stage3.h"

class Enemy : public Entity
{
public:
	static Mesh mesh;
	static Texture tex;

	Enemy();
	void draw(Shader& shader);
	void update(float dt);

	float getMoveSpeed();

	bool isAlive();
	void kill();
private:
	static bool meshInit;
	static bool texInit;

	float moveSpeed;
	bool alive;
};

#endif 