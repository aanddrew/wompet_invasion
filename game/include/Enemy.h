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
	static glm::mat4 rotation;

	Enemy();
	void draw(Shader& shader);
	void update(float dt);
private:
	static bool meshInit;
	static bool texInit;
};

#endif 