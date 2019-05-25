#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();

	void rotateUp(float dTheta);
	void rotateRight(float dTheta);

	void translate(const glm::vec3& translation);
	void setPos(glm::vec3 newPosition);

	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::vec3 getUp();
	glm::vec3 getPos();

	glm::mat4 getMatrix();
private:
	void normalizeFrame();
	//location of camera
	glm::vec3 pos;

	//our camera's frame
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
};

#endif