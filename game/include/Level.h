#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "../../utils/include/PhysTriangle.h"

class Level
{
public:
	//level file format
	Level(const std::string& fileName);

private:
	//"walls" being any piece of collidable geometry in the level
	std::vector<PhysTriangle> walls;
};

#endif