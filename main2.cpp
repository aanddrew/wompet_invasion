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

	SDL_GL_SetSwapInterval(1);

	Game game;

	//time delta
	std::chrono::high_resolution_clock clock;
	auto lastTime = clock.now();

	float dt;

	bool playing = true;
	SDL_Event e;

	while(playing)
	{
		//getting the dt - holy shit this is a verbose call
		int ms = std::chrono::duration_cast<std::chrono::microseconds>(clock.now() - lastTime).count();
		lastTime = clock.now();
		//1,000,000 microseconds in a second
		dt = ((float)ms)/1000000; //this is in seconds
		//fps counter in terminal
		// std::cout << "fps: " << 1.0/dt << std::endl;
		//polling events
		while(SDL_PollEvent(&e))
		{
	    switch(e.type)
	    {
	      case SDL_QUIT:
	        playing = false;
	      break;
	      case SDL_KEYDOWN:
	      	if (e.key.keysym.sym == SDLK_ESCAPE)
	      		game.togglePause();
	      break;
	    }
	    if(!game.isPaused())
	    	game.handleInputEvent(e);

	  }//end while poll event

	  if(!game.isPaused())
	  {
	  	SDL_ShowCursor(0);
	  	SDL_WarpMouseInWindow(window.getSDLWindow(), WIDTH/2, HEIGHT/2);
	  }
	  else
	  {
	  	SDL_ShowCursor(1);
	  }
	  // while(SDL_PollEvent(&e));

	  game.update(dt);


	  window.clear();
	  game.draw();
	  window.render();
	}//end while playing
}