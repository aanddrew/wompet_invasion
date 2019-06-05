#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "Entity.h"
#include "../../stage3/stage3.h"
#include <glm/glm.hpp>

class PhysTriangle
{
public:
	PhysTriangle(float * start, glm::vec3(outwardNormal));
public:
	//these are the points that are in the middle of the edges of the triangle
	glm::vec3 edgePoints[3];
	glm::vec3 edgeNormals[3];
	glm::vec3 points[3];

	//these is the center of the triangle
	glm::vec3 centerPoint;
	glm::vec3 centerNormal;
};
//end phystriangle


class Level
{
public:
	//level file format
	Level(const std::string& fileName);

	//collides an entity with the level
	void collide(Entity* e, float dt);

	void draw(Shader& shader);

	std::vector<glm::vec3>& getSpawns();

private:
	Texture* tex;
	Mesh* mesh;
	//"walls" being any piece of collidable geometry in the level
	std::vector<PhysTriangle> walls;

	std::vector<glm::vec3> spawns;
};

#endif