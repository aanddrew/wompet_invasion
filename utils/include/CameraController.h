#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "../../stage3/graphics/include/Camera.h"

class CameraController
{
public:
	CameraController(Camera* camIn);
	void keyInput(int key , bool pressed);
	void mouseInput(int dx, int dy);

	void update(float dt);
private:

	enum 
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
		FORWARD,
		BACK,
		NUM_MOVES,
	};

	bool moving[6];

	Camera* camera;
};

#endif