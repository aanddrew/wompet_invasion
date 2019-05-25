#ifndef GAME_H
#define GAME_H

#include "Enemy.h"
#include "Player.h"
#include "Level.h"

class Game
{
public:
	void handleInputEvent(SDL_Event e);
	void update(float dt);
	void draw();
private:
	bool paused;

	Level level;

	//tricky part, creating the level of enemies
	std::vector<Enemy> enemies;
	Player player;
};

#endif 