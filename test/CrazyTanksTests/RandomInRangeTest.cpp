#include "pch.h"
#include "gmock/gmock.h"

// Because GameEngine includes a lot of statics
#include "Bullet.cpp"
#include "GameObject.cpp"
#include "Pawn.cpp"
#include "Wall.cpp"

#include "GameEngine.cpp"

using ::testing::AllOf;
using ::testing::Ge;
using ::testing::Le;

TEST(CrazyTanks, RandomNumberInRange) {

	GameEngine* engine = GameEngine::getInstance();
	
	EXPECT_THAT(engine->getRandomInRange(0, 3), AllOf(Ge(0), Le(3)));
	//EXPECT_THAT(engine->getRandomInRange(5, 10), AllOf(Ge(0), Le(3)));
}