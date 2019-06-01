#include "../include/Game.h"

#include <iostream>
#include <thread>

#define numEnemies 100

Game::Game(SDL_Window* windowIn, int widthIn,  int heightIn)
:
level("game/res/levels/level.lvl"),
lightingShader("game/res/shaders/basicVertexShader.GLSL", "game/res/shaders/basicFragmentShader.GLSL"),
pc(&player),
ec(&enemies, &player)
{
	this->window = windowIn;
	width = widthIn;
	height = heightIn;

	paused = false;

	lightingShader.bind();
	//create our shader stuff 
	lightingShader.createUniform("camera");
	lightingShader.createUniform("model");

	//our view settings
	fov = 70.0f;
	ar = (16.0f)/(9.0f);
	projection = glm::perspective(fov, ar, 0.01f, 200.0f);
	lightingShader.setUniform("camera", projection*(player.getCamera().getMatrix()));

	//create our enemies array
	enemies.reserve(numEnemies);
	for(int i = 0; i < numEnemies; i++)
	{
		float mod1 = rand()%1000 * 1.0f/500.0f - 0.5f;
		float mod2 = rand()%1000 * 1.0f/500.0f - 0.5f;
		float mod3 = rand()%1000 * 1.0f/500.0f - 0.5f;

		enemies.push_back(Enemy());
		enemies[i].setPos(glm::vec3(0.0f, 0.0f, 0.5f));
		enemies[i].setVelocity(glm::vec3(1.0f* mod1, 1.0f* mod2, 1.0f * mod3));
	}
	// enemies.push_back(Enemy());
	// enemies.at(0).setPos(glm::vec3(0.0f, 0.0f, 0.0f));
	// enemies.at(0).setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Game::handleInputEvent(const SDL_Event& e)
{
	switch(e.type)
	{
		//keyboard inputs
		case SDL_KEYDOWN:
		  pc.keyInput(e.key.keysym.sym, true);
		break;
		case SDL_KEYUP:
		  pc.keyInput(e.key.keysym.sym, false);
		break;

		//mouse inputs
		case SDL_MOUSEBUTTONDOWN:
		  // mouseClicked = true;
		break;
		case SDL_MOUSEBUTTONUP:
		  // mouseClicked = false;
		break;
		//antiquated
		case SDL_MOUSEMOTION:
		{
		}
		break;
	}
}

void Game::update(float dt)
{
	if (paused)
	{
		return;
	}

	player.setVelocity(player.getVelocity() + gravity*dt);
	player.update(dt);

	level.collide((Entity*) &player, dt);
	// std::cout << player.getVelocity()[1] << std::endl;
	for(int i = 0; i < enemies.size(); i++)
	{
		// level.collide((Entity*) &enemies.at(i), dt);
		enemies.at(i).update(dt);
	}

	//handling mouse input

	int x,y;
	SDL_GetMouseState(&x, &y);

	// std::cout << "x: " << x << ", y: " << y << std::endl;
	int dx = x - (width/2);
	int dy = y - (height/2);

	pc.mouseInput(dx, dy);
	pc.update(dt);

	if (dx != 0 || dy != 0)
		SDL_WarpMouseInWindow(window, width/2, height/2);

	//mouse input handled

	//if the round is over, increment round
}

void Game::draw()
{
	lightingShader.bind();
	lightingShader.setUniform("camera", projection*(player.getCamera().getMatrix()));
	// std::cout << player.getCamera().getForward()[0] << player.getCamera().getForward()[1] << player.getCamera().getForward()[2] << std::endl;

	// std::cout << "cam: " << player.getCamera().getPos()[0] << player.getCamera().getPos()[1] << player.getCamera().getPos()[2] << std::endl;
	for(int i = 0; i < enemies.size(); i++)
	{
		// std::cout << enemies.at(i).getPos()[0] << enemies.at(i).getPos()[1] << enemies.at(i).getPos()[2] << std::endl;
		enemies.at(i).draw(lightingShader);
	}

	level.draw(lightingShader);
}

bool Game::isPaused() {return paused;}

void Game::togglePause()
{
	std::cout << "paused: " << (paused ? "yes" : "no") << std::endl;
	paused = !paused;
}