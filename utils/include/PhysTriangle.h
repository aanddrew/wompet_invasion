#ifndef PHYSTRIANGLE_H
#define PHYSTRIANGLE_H

#include <glm/glm.hpp>

class PhysTriangle
{
public:
	PhysTriangle(float * start, glm::vec3 outwardNormal);
private:
	//these are the points that are in the middle of the edges of the triangle
	glm::vec3 edgePoints[3];
	glm::vec3 edgeNormals[3];

	//these is the center of the triangle
	glm::vec3 centerPoint;
	glm::vec3 centerNormal;
};

#endif