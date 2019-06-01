#ifndef GAME_H
#define GAME_H

#include "Enemy.h"
#include "Player.h"
#include "Level.h"
#include "Entity.h"

#include "../../utils/include/EnemyController.h"
#include "../../utils/include/PlayerController.h"

#include "../../stage3/stage3.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <vector>

class Game
{
public:
	Game(SDL_Window* window, int, int);

	void handleInputEvent(const SDL_Event& e);
	void update(float dt);
	void draw();

	void togglePause();
	bool isPaused();

	//gravitational constant vector, magnitude is in units/s/s
	glm::vec3 gravity = glm::vec3(0.0f, -32.0f, 0.0f);

private:
	//shader for the game
	Shader lightingShader;

	bool paused;
	unsigned int round;

	PlayerController pc;
	EnemyController ec;

	//this class is going to be pretty complicated
	Level level;

	//tricky part, creating the level of enemies
	std::vector<Enemy> enemies;
	Player player;

	SDL_Window* window;
	int width;
	int height;

	glm::mat4 projection;
	float fov;
	float ar;
};

#endif 