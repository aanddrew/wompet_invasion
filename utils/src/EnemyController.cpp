#include "../include/EnemyController.h"

EnemyController::EnemyController(std::vector<Enemy>* enemiesIn, Player* playerIn)
{
	enemies = enemiesIn;
	player = playerIn;
}