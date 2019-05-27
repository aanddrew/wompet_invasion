#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

#include <vector>

#include "../../game/include/Enemy.h"
#include "../../game/include/Player.h"

class EnemyController
{
public:
	EnemyController(std::vector<Enemy>* enemiesIn, Player* playerIn);
private:
	//ptr to vector inside of game
	std::vector<Enemy>* enemies;
	//used to determine path to travel
	Player* player;
};

#endif