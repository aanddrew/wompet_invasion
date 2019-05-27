#include "../include/Enemy.h"

#include <iostream>

Mesh Enemy::mesh = Mesh("game/res/models/wompet.obj");
Texture Enemy::tex = Texture("game/res/textures/wompet.png");
bool Enemy::meshInit = false;
bool Enemy::texInit = false;
// Mesh Enemy::mesh = Mesh();


glm::mat4 Enemy::rotation = glm::mat4(1.0f);

Enemy::Enemy()
{
	setPos(glm::vec3(0.0f, 0.0f, 0.0f));
	setVelocity(glm::vec3(1.0f, 0.1f, 0.5f));

	if (!meshInit)
	{
		std::cout << "Loading enemy mesh into graphics memory." << std::endl;
		mesh.load();
		meshInit = true;
	}
	if(!texInit)
	{
		std::cout << "Loading enemy texture into graphics memory." << std::endl;
		tex.load();
		texInit = true;
	}

	// std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	// mesh = Mesh("../../res/models/normalCube.obj");
}

void Enemy::draw(Shader& shader)
{
	shader.bind();
	shader.setUniform("model", glm::translate(glm::mat4(1.0f), this->getPos()) *  rotation);

	// std::cout << "getPos(): " << getPos()[0] << ", " << getPos()[1] << ", " << getPos()[2] << std::endl;

	tex.bind();
	mesh.bind();

	mesh.draw();
}

void Enemy::update(float dt)
{
	setPos(getPos() + dt*getVelocity());
}