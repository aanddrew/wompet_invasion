#include "../include/EnemyController.h"

#include <iostream>

EnemyController::EnemyController()
{
	enemies = nullptr;
	player  = nullptr;
	spawns  = nullptr;
}

EnemyController::EnemyController(std::vector<Enemy>* enemiesIn, Player* playerIn, std::vector<glm::vec3>* spawnsIn)
{
	enemies = enemiesIn;
	player = playerIn;
	spawns = spawnsIn;

	std::cout << "spawns size: " << spawns->size() << std::endl;
	for(int i = 0; i < spawns->size(); i++)
	{
		std::cout << (i+1) % spawns->size() << std::endl;
		spawnDistances.push_back(glm::length(   spawns->at(i) - spawns->at((i+1) % spawns->size())   ));
	}
	std::cout << "dist size: " << spawnDistances.size() << std::endl;
}

void collide(Entity* e1, Entity* e2)
{
	glm::vec3 dist = e2->getPos() - e1->getPos();

	if (glm::length(dist) < e2->getRadius() + e1->getRadius())
	{
		e1->setPos(e1->getPos() + glm::normalize(dist) * e2->getRadius());
	}
}

//returns x mod base, but without negative nubmers
unsigned long modHelper(int x, int base)
{
	if (x < 0)
		return (unsigned long)(base + (x%base));
	else
		return (unsigned long)(x%base);
}

void EnemyController::update(float dt)
{
	//have the enemies collide into eachotehr

	static const int numCollisions = 100;

	int spawnClosestToPlayer = 0;
	float tempDist = glm::length(player->getPos() - spawns->at(0));
	for(int i = 1; i < spawns->size(); i++)
	{
		// std::cout << "i: " << i << std::endl;
		float dist = glm::length( player->getPos() - spawns->at(i)  );
		if (dist < tempDist)
		{
			tempDist = dist;
			spawnClosestToPlayer = i;
		}
	}

	int destIndex = spawnClosestToPlayer;
	// std::cout << "destIndex " << destIndex << std::endl;

	for(int i = 0; i < enemies->size(); i++)
	{
		glm::vec3 dest = player->getPos();
		glm::vec3 start = enemies->at(i).getPos();

		//if they are close to the player, they will go directly towards them
		//otherwise they will aim slightly off
		glm::vec3 vec = dest - start;

		float playerDist = glm::length(vec);

		//now - find the two second closest spawn points
		//if the closest of those two is closer than the player, travel to the one that 
		//brings the enemy closer to the player

		int closeIndeces[3] ={-1, -1, -1};
		float closeDistances[3] = {1000.0f, 1000.0f, 1000.0f};
		for(int k = 0; k < spawns->size(); k++)
		{
			float dist = glm::length(spawns->at(k) - start);
			for(int j = 0; j < 3; j++)
			{
				//we have to sift it upwards
				if (closeIndeces[j] == -1 || dist < closeDistances[j])
				{
					//insert at slot j
					int closeIndecesCopy[3] = {closeIndeces[0], closeIndeces[1], closeIndeces[2]};
					float closeDistancesCopy[3] = {closeDistances[0], closeDistances[1], closeDistances[2]};

					closeDistancesCopy[j] = dist;
					closeIndecesCopy[j] = k;

					for(int sort = j+1; sort < 3; sort++)
					{
						closeDistancesCopy[sort] = closeDistances[sort-1];
						closeIndecesCopy[sort] = closeIndeces[sort-1];
					}

					for(int a = 0; a < 3; a++)
					{
						closeIndeces[a] = closeIndecesCopy[a];
						closeDistances[a] = closeDistancesCopy[a];
					}
					break;
				}
			}
		}

		int TOLERANCE = 0;

		//a, b are the two second closest spawn points to the enemy
		int a = closeIndeces[0];

		// std::cout << "a: " << a << " dest: " << destIndex << std::endl;

		bool special = false;
		if (abs(a - destIndex) <= TOLERANCE || abs(a- destIndex) >= spawnDistances.size() - TOLERANCE)
			special = true;

		if (!special)
		{
			float firstDist = tempDist;
			float secondDist = tempDist;

			//f is first, s is second
			for(int f = a; f != destIndex;   )
			{
				// std::cout << "f: " << f << std::endl;
				firstDist += spawnDistances.at(f);

				if (f == spawns->size() -1)
					f = 0;
				else
					f++;
			}
			// if (destIndex == 10)
			// 	std::cout << "s now" << std::endl;
			for(int s = a; s != destIndex;   )
			{
				int idx = (s > 1 ? s - 1 : spawnDistances.size() - 1);
				firstDist += spawnDistances.at( idx );

				if (s == 0)
					s = spawns->size() - 1;
				else
					s--;
			}

			// if (destIndex == 10)
			// 	std::cout << "here" << std::endl;

			// std::cout << "end" << std::endl;

			//hypothetical travel vectors
			// glm::vec3 hypoTravel1 = spawns->at(a) - enemies->at(i).getPos();
			// glm::vec3 hypoTravel2 = spawns->at(b) - enemies->at(i).getPos();

			//check if the player is closer than either of them
			if (playerDist > firstDist && playerDist > secondDist)
			{
				//if the player is too far away, we do the AI routing
				if (firstDist < secondDist)
					vec = spawns->at(modHelper(a+1, spawns->size())) - enemies->at(i).getPos();
				else
					vec = spawns->at(modHelper(a-1, spawns->size())) - enemies->at(i).getPos();
			}	
		}
		
		vec = glm::normalize(vec);
		enemies->at(i).setVelocity(vec*enemies->at(i).getMoveSpeed());
	}
}