#include "stage3/stage3.h"
#include "game/gameIncludes.h"
#include "utils/include/CameraController.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>

#define TITLE "Game Title"
#define WIDTH 1600
#define HEIGHT 900

#define FOV 70.0f
#define NEAR 0.05f
#define FAR 200.0f
#define AR (WIDTH*1.0f/HEIGHT)

int main()
{
	srand(time(0));
	s3::Window window(TITLE, WIDTH, HEIGHT);
	window.setBG(glm::vec3(0.1f, 0.15f, 0.3f));

	//creation of shaders and graphics objects
	Shader lightingShader("game/res/shaders/basicVertexShader.GLSL", "game/res/shaders/basicFragmentShader.GLSL");
	lightingShader.bind();

	Camera cam;
	CameraController camControl(&cam);
	// cam.translate(glm::vec3(0.0f, 5.0f, 0.0f));
	glm::mat4 perspective = glm::perspective(FOV, AR, NEAR, FAR);
	lightingShader.createUniform("camera");
	lightingShader.setUniform("camera", perspective*cam.getMatrix());

	//the model will just be an identity
	lightingShader.createUniform("model");
	lightingShader.setUniform("model", glm::mat4(1.0f));

	//testing
	// Mesh cube("res/models/normalCube.obj");
	// cube.bind();

	//creation of game objects

	Enemy enemy;
	Enemy enemy2;

	#define numEnemies 100
	Enemy enemies[numEnemies];
	for(int i = 0; i < numEnemies; i++)
	{
		float mod1 = rand()%1000 * 1.0f/500.0f - 0.5f;
		float mod2 = rand()%1000 * 1.0f/500.0f - 0.5f;
		float mod3 = rand()%1000 * 1.0f/500.0f - 0.5f;

		enemies[i] = Enemy();
		enemies[i].setPos(glm::vec3(0.0f, 0.0f, 0.5f));
		enemies[i].setVelocity(glm::vec3(1.0f* mod1, 1.0f* mod2, 1.0f * mod3));
	}

	//time delta
	std::chrono::high_resolution_clock clock;
	auto lastTime = clock.now();

	float dt;


	//game stuff
	bool playing = true;
	SDL_Event e;

	bool mouseClicked = false;

	while(playing)
	{
		//getting the dt - holy shit this is a verbose call
		int ms = std::chrono::duration_cast<std::chrono::microseconds>(clock.now() - lastTime).count();
		lastTime = clock.now();
		//1,000,000 microseconds in a second
		dt = ((float)ms)/1000000; //this is in seconds

		std::cout << "fps: " << 1.0/dt << std::endl;
		//polling events
		while(SDL_PollEvent(&e))
		{
	    switch(e.type)
	    {
	      case SDL_QUIT:
	        playing = false;
	      break;

	      //keyboard inputs
	      case SDL_KEYDOWN:
	        camControl.keyInput(e.key.keysym.sym, true);
	      break;
	      case SDL_KEYUP:
	        camControl.keyInput(e.key.keysym.sym, false);
	      break;

	      //mouse inputs
	      case SDL_MOUSEBUTTONDOWN:
	        mouseClicked = true;
	      break;
	      case SDL_MOUSEBUTTONUP:
	        mouseClicked = false;
	      break;
	      case SDL_MOUSEMOTION:
	        if (mouseClicked)
	          camControl.mouseInput(e.motion.xrel, e.motion.yrel);
	      break;
	    }
	  }
	  camControl.update(dt);
	  lightingShader.setUniform("camera", perspective*cam.getMatrix());


	  // enemy.update(dt);

	  //drawing takes place here
	  window.clear();

	  lightingShader.bind();

	  enemy.draw(lightingShader);

	  for(int i = 0; i < numEnemies; i++)
	  {
	  	enemies[i].update(dt);
	  	enemies[i].draw(lightingShader);
	  }

	  window.render();
	}

	return 0;
}