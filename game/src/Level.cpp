#include "../include/Level.h"

#include <fstream>
#include <iostream>

#include <glm/gtc/quaternion.hpp>

#define PI 3.14159265f

void printVec(const glm::vec3& vec)
{
	std::cout << "[" << vec[0] << ", " << vec[1] << ", " << vec[2] << "]" << std::endl;
}

PhysTriangle::PhysTriangle(float* start, glm::vec3(outwardNormal))
{
	//counter clockwise is the default
	glm::vec3 points[3];
	for(int i = 0; i < 3; i++)
	{
		points[i] = glm::vec3(start[3*i + 0], start[3*i + 1], start[3*i + 2]);
	}

	//setting the edge points
	edgePoints[0] = (points[0] + points[1])/2.0f;
	edgePoints[1] = (points[1] + points[2])/2.0f;
	edgePoints[2] = (points[2] + points[0])/2.0f;

	//grabbing their normals
	//this is a means of getting the vector from a->b and rotating it 90 degrees cw 
	//then normalizing it

	//omg... angleAxis is in radians, not degrees. FACEPALM
	glm::quat rotation = glm::angleAxis(PI/2.0f, outwardNormal);
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

	glm::vec3 norm1 = points[1] - points[0];
	glm::vec3 norm2 = points[2] - points[1];
	glm::vec3 norm3 = points[0] - points[2];
	//rotate it 90* CW about outwardNormal Axis

	norm1 = rotationMatrix*(glm::vec4(norm1, 1.0));
	norm2 = rotationMatrix*(glm::vec4(norm2, 1.0));
	norm3 = rotationMatrix*(glm::vec4(norm3, 1.0));

	//normalize them
	norm1 = glm::normalize(norm1);
	norm2 = glm::normalize(norm2);
	norm3 = glm::normalize(norm3);

	//set our data members
	edgeNormals[0] = norm1;
	edgeNormals[1] = norm2;
	edgeNormals[2] = norm3;

	//center point gang - average of all points
	centerPoint = (points[0] + points[1] + points[2])/3.0f;
	centerNormal = outwardNormal;
}

//END PHYSTYRIANGLE STUFF

//this file is the level file
Level::Level(const std::string& fileName)
{
	std::string temp;
	std::ifstream inFile(fileName);

	//loading mesh
	std::getline(inFile, temp);
	std::cout << temp << std::endl;
	mesh = new Mesh(temp);
	std::cout << "Loading level mesh into graphics memory" << std::endl;
	mesh->load();

	// std::cout << mesh.getVertices()[0] << mesh.getVertices()[1] << mesh.getVertices()[2] << std::endl;

	//loading texture
	std::getline(inFile, temp);
	tex = new Texture(temp);
	std::cout << "Loading level texture into graphics memory" << std::endl;
	tex->load();


	//not a copy operation - I think
	std::vector<float> vertexArray = mesh->getVertices();
	std::vector<float> normalArray = mesh->getNormals();

	//the two vectors should be the same size
	for(int i = 0; i < vertexArray.size()/9; i++)
	{
		glm::vec3 norm = glm::vec3(normalArray[i*9 + 0], normalArray[i*9 + 1], normalArray[i*9 + 2]);
		walls.push_back(PhysTriangle(&(vertexArray[i*9]), norm));
	}

	//now we have the list of walls in the level
	//let's turn it into a spatial hash map
	//first, find out how big the map is. Largest x, y, and z value

	//PROBLEM: we need to check all points on the triangle, not just the center

	glm::vec3 largest(-1000.0f, -1000.0f, -1000.0f);
	glm::vec3 smallest(1000.0f, 1000.0f, 1000.0f);
	for(int i = 0; i < walls.size(); i++)
	{
		if (walls.at(i).centerPoint[0] < smallest[0])
			smallest[0] = walls.at(i).centerPoint[0];
		if (walls.at(i).centerPoint[1] < smallest[1])
			smallest[1] = walls.at(i).centerPoint[1];
		if (walls.at(i).centerPoint[2] < smallest[2])
			smallest[2] = walls.at(i).centerPoint[2];

		if (walls.at(i).centerPoint[0] > largest[0])
			largest[0] = walls.at(i).centerPoint[0];
		if (walls.at(i).centerPoint[1] > largest[1])
			largest[1] = walls.at(i).centerPoint[1];
		if (walls.at(i).centerPoint[2] > largest[2])
			largest[2] = walls.at(i).centerPoint[2];
	}
	//we now have a bounding cuboid

	float sizeXFloat = largest[0] - smallest[0];
	float sizeYFloat = largest[1] - smallest[1];
	float sizeZFloat = largest[2] - smallest[2];

	//the +2 is padding because the conversion pads it down, we must pad it back up
	size_x = (int)(sizeXFloat + 2) / LEVEL_GRANULARITY;
	size_y = (int)(sizeYFloat + 2) / LEVEL_GRANULARITY;
	size_z = (int)(sizeZFloat + 2) / LEVEL_GRANULARITY;

	int START_X = smallest[0] / LEVEL_GRANULARITY;		
}

void Level::collide(Entity* e, float dt)
{
	// std::cout << "walls size: " << walls.size() << std::endl;
	for(int i = 0; i < walls.size(); i++)
	{
		glm::vec3 nextPos = e->getPos() + (e->getVelocity()*dt);
		bool collision = true;
		// std::cout << "new wall" << std::endl;
		// std::cout << "Hi" << std::endl;
		//check if player is inside triangle
		for(int j = 0; j < 3 && collision; j++)
		{
			if (glm::dot( walls.at(i).edgeNormals[j], (nextPos - walls.at(i).edgePoints[j]) ) < 0.0f)
			{
				collision = false;
			}
		}

		if (!collision)
			continue;

		// if (collision)
		// 	std::cout << "inside" << std::endl;

		float centerDot = glm::dot(walls.at(i).centerNormal, (nextPos - walls.at(i).centerPoint));
		//now we check the dot with centernorm
		if (abs(centerDot) > e->getRadius())
			collision = false;

		if (collision)
		{
			//this is the distance between the plane and the edge of the sphere
			//draw a diagram
			float dr = abs(e->getRadius()) - abs(centerDot); 
			e->setPos((e->getPos() + (walls.at(i).centerNormal * dr)));

			float sinAngleFromVertical = glm::dot(walls.at(i).centerNormal, glm::vec3(0.0f, 1.0f, 0.0f));

			// std::cout << angleFromVertical << std::endl;
			//floor collision
			if (sinAngleFromVertical > 0.5)	
				e->setVelocity(glm::vec3(e->getVelocity()[0], 0.0f, e->getVelocity()[2]));
			//ceiling collision
			else if (sinAngleFromVertical < -0.5f && e->getVelocity()[1] > 0.0f)
				e->setVelocity(glm::vec3(e->getVelocity()[0], -2.0f, e->getVelocity()[2]));
		}
	}
}

void Level::draw(Shader& shader)
{
	shader.bind();
	shader.setUniform("model", glm::mat4(1.0f));
	mesh->bind();
	tex->bind();

	mesh->draw();
}