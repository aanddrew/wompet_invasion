#include "../include/Enemy.h"

#include <iostream>

Mesh Enemy::mesh = Mesh("game/res/models/wompet.obj");
Texture Enemy::tex = Texture("game/res/textures/wompet.png");
bool Enemy::meshInit = false;
bool Enemy::texInit = false;
// Mesh Enemy::mesh = Mesh();

Enemy::Enemy()
{
	alive = true;
	setRadius(1.0f);

	float x = (rand()%100)*1.0f;
	float y = (rand()%100)*1.0f;
	float z = (rand()%100)*1.0f;

	// std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

	setPos(glm::vec3(x,y,z));
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

	int r = rand()%1000;
	r += 950;
	float mod = (r*1.0f)/1000.0f;

	moveSpeed = mod * 5.0f;

	// std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	// mesh = Mesh("../../res/models/normalCube.obj");
}

float Enemy::getMoveSpeed()
{
	return moveSpeed;
}

bool Enemy::isAlive()
{
	return alive;
}

void Enemy::kill()
{
	alive = false;
}

void Enemy::draw(Shader& shader)
{
	static int drawn = 0;

	if (drawn < 1)
	{
		float x = (rand()%50)*1.0f;
		float y = (rand()%50)*1.0f;
		float z = (rand()%50)*1.0f;

		// std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

		setPos(glm::vec3(x,y,z));
		drawn++;
	}
	shader.bind();


	//https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
	//copied this from here. I really don't understand the linear algebra behind this
	glm::vec3 a(0.0f, 0.0f, 1.0f);
	glm::vec3 b = glm::normalize(this->getVelocity());

	glm::vec3 v = glm::cross(a,b);

	float s = glm::length(v); //sine of angle
	float c = glm::dot(a, b); //cosine of angle

	glm::mat3 vx
	(
		glm::vec3(0, v[2], -1*v[1]),
		glm::vec3(-1*v[2], 0, v[0]),
		glm::vec3(v[1], -1*v[0], 0)
	);

	glm::mat3 rotate = glm::mat3(1.0f) + vx + (vx*vx*(1.0f/(1.0f+c)));

	shader.setUniform("model", glm::translate(glm::mat4(1.0f), this->getPos()) *  glm::mat4(rotate));

	// std::cout << "getPos(): " << getPos()[0] << ", " << getPos()[1] << ", " << getPos()[2] << std::endl;

	tex.bind();
	mesh.bind();

	mesh.draw();
}

void Enemy::update(float dt)
{
	setPos(getPos() + dt*getVelocity());
}