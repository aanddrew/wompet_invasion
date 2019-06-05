#include "../include/Level.h"

#include <fstream>
#include <sstream>
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
	// glm::vec3 points[3];
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

//helpful for parsing the file, thanks to my intro cs course for this function
void levelSplit(std::string temp, std::string words[], char del, int maxWords)
{
	std::stringstream tempStream(temp);
	int i = 0;
	std::string tempFromTemp;
	while(std::getline(tempStream, tempFromTemp, del))
	{
		if (i == maxWords)
			return;
		words[i] = tempFromTemp;
		i++;
	}
}

//END HELPERS=================================
//==========================================

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

	std::getline(inFile, temp);

	if (temp[0] == 's')
	{
		while(std::getline(inFile,temp))
		{
			if (temp == "/s")
				break;

			std::string nums[3];

			levelSplit(temp, nums, ',', 3);

			spawns.push_back(
				glm::vec3
				(
					std::stof(nums[0]), 
					std::stof(nums[1]), 
					std::stof(nums[2])
				)
			);
			//adds a spawn point at this vector
		}
	}


	//not a copy operation - I think
	std::vector<float> vertexArray = mesh->getVertices();
	std::vector<float> normalArray = mesh->getNormals();

	//the two vectors should be the same size
	for(int i = 0; i < vertexArray.size()/9; i++)
	{
		glm::vec3 norm = glm::vec3(normalArray[i*9 + 0], normalArray[i*9 + 1], normalArray[i*9 + 2]);
		walls.push_back(PhysTriangle(&(vertexArray[i*9]), norm));
	}
}

void Level::collide(Entity* e, float dt)
{
	// std::cout << "x: " << e->getPos()[0] << " y: " << e->getPos()[1] << " z: " << e->getPos()[2] << std::endl;
	// glm::vec3 spawn1 = spawns[0];
	// std::cout << glm::length(e->getPos() - spawn1) << std::endl;

	//this method could have been improved through spatial hashing
	//however, I have failed in different attempts to get it to work,
	//so the game is being changed so that the only entity colliding with the level
	//is the player themselves

	for(int i = 0; i < walls.size(); i++)
	{
		glm::vec3 nextPos = e->getPos() + (e->getVelocity()*dt);
		bool collision = true;

		float centerDot = glm::dot(walls.at(i).centerNormal, (nextPos - walls.at(i).centerPoint));
		//now we check the dot with centernorm
		if (abs(centerDot) > e->getRadius())
			continue;

		//check if player is inside triangle
		for(int j = 0; j < 3 && collision; j++)
		{
			if (glm::dot( walls.at(i).edgeNormals[j], (nextPos - walls.at(i).edgePoints[j]) ) < 0.0f)
			{
				collision = false;
			}
		}

		//if its not a collsion we can move on
		if (!collision)
			continue;
		else
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
	} // end for every wall
}

std::vector<glm::vec3>& Level::getSpawns()
{
	return spawns;
}

void Level::draw(Shader& shader)
{
	shader.bind();
	shader.setUniform("model", glm::mat4(1.0f));
	mesh->bind();
	tex->bind();

	mesh->draw();
}