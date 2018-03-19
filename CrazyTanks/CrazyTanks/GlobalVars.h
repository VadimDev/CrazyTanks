#pragma once

#include "GameObject.h"
#include "GameEngine.h"

// game map size
static const int MAP_HEIGHT = 20;
static const int MAP_WIDTH = 50;

static const int taskRange = 4;

// 2D gaming data array
static GameObject * gameData[MAP_HEIGHT][MAP_WIDTH];

static GameEngine * engine = new GameEngine;

enum gameObjects { field, wall, enemy, bullet, player, border };
enum tasks { t_forward, t_backward, t_left, t_right, t_fire };