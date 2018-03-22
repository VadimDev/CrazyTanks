#include "pch.h"
#include "gmock/gmock.h"

#include "Wall.h"

TEST(CrazyTanks, WallDestruction) {
	Wall* wall = new Wall;

	ASSERT_EQ(wall->intact, wall->getWallState());
	std::cout << "WALL STATE: INTACT WALL..HIT WALL" << std::endl;

	wall->hitWall();

	ASSERT_EQ(wall->damaged, wall->getWallState());

	std::cout << "NOW THE WALL IS DAMAGED" << std::endl;
}