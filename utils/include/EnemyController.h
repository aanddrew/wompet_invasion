#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

#include <vector>

#include "../../game/include/Enemy.h"
#include "../../game/include/Player.h"

class EnemyController
{
public:
	EnemyController();
	EnemyController(std::vector<Enemy>* enemiesIn, Player* playerIn, std::vector<glm::vec3>* spawnsIn);

	void update(float dt);
private:
	//ptr to vector inside of game
	std::vector<Enemy>* enemies;
	//used to determine path to travel
	Player* player;

	std::vector<glm::vec3>* spawns;
	std::vector<float> spawnDistances;

	float moveSpeed = 5.0f;
};

#endif